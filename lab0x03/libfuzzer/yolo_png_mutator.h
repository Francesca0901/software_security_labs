// Copyright 2019 Google Inc. All Rights Reserved.
// Licensed under the Apache License, Version 2.0 (the "License");
#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cassert>
#include <random>

#include <zlib.h>

// A simple class for parsing, serializing, and mutating an PNG file.
// https://en.wikipedia.org/wiki/Portable_Network_Graphics
// It is an example of a custom mutator for libFuzzer
// (https://llvm.org/docs/LibFuzzer.html) used for
// "structure-aware coverage-guided fuzzing".
//
// If you have a non structure-aware fuzz target for any API that handles
// PNG inputs, you can turn that fuzz target into a structure-aware one
// by defining PNG_MUTATOR_DEFINE_LIBFUZZER_CUSTOM_MUTATOR and then
// including this file.
class YoloPngMutator {
  using V = std::vector<uint8_t>;

  struct Chunk {
    uint32_t type;
    V v;
  };

 public:

  // Parse the input stream as a PNG file,
  // put every chunk into its own vector,
  // uncompress chunk data when needed,
  // merge the IDAT chunks into one vector.
  YoloPngMutator(std::istream &in) {
    ihdr_.resize(13);
    Read4(in);
    Read4(in); // Skip the 8-byte magic value.
    // read IHDR.
    if (ReadInteger(in) != 13) return;
    if (Read4(in) != Type("IHDR")) return;
    // Read 13 values.
    in.read((char*)ihdr_.data(), ihdr_.size());
    Read4(in);  // ignore CRC
    ssize_t idat_idx = -1;

    while (in) {
      uint32_t len = ReadInteger(in);
      uint32_t type = Read4(in);
      if (type == Type("IEND")) break;  // do nothing
      
      char chunk_name[5];
      memcpy(chunk_name, &type, 4);
      chunk_name[4] = 0;
      if (len > (1 << 20)) return;
      V v(len);
      in.read((char *)v.data(), len);
      Read4(in);  // ignore CRC

      if (type == Type("PLTE")) {
        chunks_.push_back({type, v});
      } else if (type == Type("IDAT")) {
        if (idat_idx != -1)
          Append(&chunks_[idat_idx].v, v);
        else {
          idat_idx = chunks_.size();
          chunks_.push_back({type, v});
        }
      } 
      // else if (type == Type("PLTE")){
        // chunks_.push_back({type, v});
      // }
      //  std::cerr << "CHUNK: " << chunk_name << std::endl;
    }
    if (idat_idx != -1)
      chunks_[idat_idx].v = Uncompress(chunks_[idat_idx].v);
  }

  // Write back the PNG file.
  void Serialize(std::ostream &out) {
    const unsigned char header[] = {0x89, 0x50, 0x4e, 0x47,
                                    0x0d, 0x0a, 0x1a, 0x0a};
    // bool chunk_idx = 0;
    out.write((const char*)header, sizeof(header));
    WriteChunk(out, "IHDR", ihdr_);

    for (auto &ch : chunks_) {
      if (ch.type == Type("PLET")){
        WriteChunk(out, ch.type, ch.v);
        break;
      }
    }

    for (auto &ch : chunks_) {
      if (ch.type == Type("IDAT")){
        WriteChunk(out, ch.type, ch.v);
      }
    }

    WriteChunk(out, "IEND", {});
  }

  // Raw byte array mutator, like that provided by libFuzzer.
  using Mutator = size_t (*)(uint8_t *Data, size_t Size, size_t MaxSize);

  // Mutate the in-memory representation of a PNG file.
  // Given the same Seed, the same mutation is performed.
  void Mutate(Mutator m, unsigned int Seed) {
    std::minstd_rand rnd(Seed);
    auto M = [&](V *v) {
      if (v->empty())
        v->resize(v->size() + 1 + rnd() % 256);
      v->resize(m(v->data(), v->size(), v->size()));
    };

    bool plte_exist = false;
    for (const auto& ch : chunks_) {
        if (ch.type == Type("PLTE")) {
        plte_exist = true;
        break;
        }
    }
    switch (rnd() % 5) {
      // Mutate IHDR.
      case 0:
        // m(ihdr_.data(), ihdr_.size(), ihdr_.size());
        m(ihdr_.data(), 8, 8); //only mutate the width and the height
        break;
      // Mutate some other chunk.
      case 1:
        if (!chunks_.empty()) M(&chunks_[rnd() % chunks_.size()].v);
        break;
      // Shuffle the chunks.
      case 2:
        // std::shuffle(chunks_.begin(), chunks_.end(), rnd);
        CustomShuffle(chunks_, rnd);
        break;
      // Delete a random chunk.
      case 3:
        if (!chunks_.empty()){
            std::vector<Chunk> idat_chunks;
            std::vector<Chunk> plte_chunks; 
            for (const auto& chunk : chunks_) {
                if (chunk.type == Type("IDAT")) {
                    idat_chunks.push_back(chunk);
                } else if (chunk.type == Type("PLTE")) {
                    plte_chunks.push_back(chunk);
                }
            }
            if (!idat_chunks.empty()) {
                idat_chunks.erase(idat_chunks.begin() + rnd() % idat_chunks.size());
            }
            // Clear the original chunks vector and merge the shuffled chunks
            chunks_.clear();
            chunks_.insert(chunks_.end(), plte_chunks.begin(), plte_chunks.end());
            chunks_.insert(chunks_.end(), idat_chunks.begin(), idat_chunks.end());
        }
        //  chunks_.erase(chunks_.begin() + rnd() % chunks_.size());
        break;
      // Insert a random chunk with one of the known types.
      case 4: {
        static const char *types[] = {
            "PLTE", "IDAT"// special chunk for extra fuzzing hints.
        };
        static const size_t n_types = sizeof(types) / sizeof(types[0]);
        uint32_t type =
            (rnd() % 10 <= 8) ? Type(types[rnd() % n_types]) : (uint32_t)rnd();

        
        size_t len = rnd() % 256;
        if (type == Type("PLTE"))
            len *= 3;
        V v(len);
        for (auto &b : v) b = rnd() % 256;

        // size_t pos = rnd() % (chunks_.size() + 1);
        size_t pos = 0;
        if (type == Type("IDAT")) {
        // Find the position of the first IDAT chunk or the end of the vector if no IDAT chunks are present
            for (; pos < chunks_.size(); ++pos) {
                if (chunks_[pos].type == Type("IDAT")) {
                    break;
                }
            }
            pos = pos + (rnd() % (chunks_.size() + 1 - pos));
            chunks_.insert(chunks_.begin() + pos, {type, v});
        } else if (type == Type("PLTE") || !plte_exist) {
        // Find the position of the first PLTE or IDAT chunk, or the end of the vector if none are present
            for (; pos < chunks_.size(); ++pos) {
                if (chunks_[pos].type == Type("PLTE") || chunks_[pos].type == Type("IDAT")) {
                    break;
                }
            }
            pos = pos + (rnd() % (chunks_.size() + 1 - pos));
            chunks_.insert(chunks_.begin() + pos, {type, v});  
        }
      } break;
    }
  }

  // Here I want to make sure shuffle won't change the order of PLET -> IDAT
  void CustomShuffle(std::vector<Chunk>& chunks, std::minstd_rand& rnd) {
    std::vector<Chunk> idat_chunks;
    std::vector<Chunk> plte_chunks;  
    // Split the chunks into separate vectors
    for (const auto& chunk : chunks) {
        if (chunk.type == Type("IDAT")) {
            idat_chunks.push_back(chunk);
        } else if (chunk.type == Type("PLTE")) {
            plte_chunks.push_back(chunk);
        }
    }

    std::shuffle(idat_chunks.begin(), idat_chunks.end(), rnd);
    std::shuffle(plte_chunks.begin(), plte_chunks.end(), rnd);

    // Clear the original chunks vector and merge the shuffled chunks
    chunks.clear();
    chunks.insert(chunks.end(), plte_chunks.begin(), plte_chunks.end());
    chunks.insert(chunks.end(), idat_chunks.begin(), idat_chunks.end());
  }

  // Takes a random IDAT chunk from p and inserts into *this.
  void CrossOver(const YoloPngMutator &p, unsigned int Seed) {
    // we only want IDAT
    std::vector<Chunk> idat_chunks;
    for (const auto& chunk : p.chunks_) {
      if (chunk.type == Type("IDAT")) {
        idat_chunks.push_back(chunk);
      }
    }
    if (idat_chunks.empty()) return;
    
    std::minstd_rand rnd(Seed);
    size_t idx = rnd() % idat_chunks.size();
    auto &ch = idat_chunks[idx];

    //just randomly pick position, we will do serialization later
    size_t pos = rnd() % (chunks_.size() + 1);
    chunks_.insert(chunks_.begin() + pos, ch); 
  }

 private:
  void Append(V *to, const V &from) {
    to->insert(to->end(), from.begin(), from.end());
  }

  uint32_t Read4(std::istream &in) {
    uint32_t res = 0;
    in.read((char *)&res, sizeof(res));
    return res;
  }
  uint32_t ReadInteger(std::istream &in) {
    return __builtin_bswap32(Read4(in));
  }
  static uint32_t Type(const char *tagname) {
    uint32_t res;
    assert(strlen(tagname) == 4);
    memcpy(&res, tagname, 4);
    return res;
  }

  void WriteInt(std::ostream &out, uint32_t x) {
    x = __builtin_bswap32(x);
    out.write((char *)&x, sizeof(x));
  }

  // Chunk is written as:
  //  * 4-byte length
  //  * 4-byte type
  //  * the data itself
  //  * 4-byte crc (of type and data)
  void WriteChunk(std::ostream &out, const char *type, const V &chunk,
                  bool compress = false) {
    V compressed;
    const V *v = &chunk;
    if (compress) {
      compressed = Compress(chunk);
      v = &compressed;
    }
    uint32_t len = v->size();
    uint32_t crc = crc32(0, (const unsigned char *)type, 4);
    if (v->size())
      crc = crc32(crc, (const unsigned char *)v->data(), v->size());
    WriteInt(out, len);
    out.write(type, 4);
    out.write((const char*)v->data(), v->size());
    WriteInt(out, crc);
  }

  void WriteChunk(std::ostream &out, uint32_t type, const V &chunk) {
    char type_s[5];
    memcpy(type_s, &type, 4);
    type_s[4] = 0;
    WriteChunk(out, type_s, chunk);
  }

  V Uncompress(const V &compressed) {
    V v;
    static const size_t kMaxBuffer = 1 << 28;
    for (size_t sz = compressed.size() * 4; sz < kMaxBuffer; sz *= 2) {
      v.resize(sz);
      unsigned long len = sz;
      auto res =
          uncompress(v.data(), &len, compressed.data(), compressed.size());
      if (res == Z_BUF_ERROR) continue;
      if (res != Z_OK) return {};
      v.resize(len);
      break;
    }
    return v;
  }

  V Compress(const V &uncompressed) {
    V v;
    static const size_t kMaxBuffer = 1 << 28;
    for (size_t sz = uncompressed.size(); sz < kMaxBuffer; sz *= 2) {
      v.resize(sz);
      unsigned long len = sz;
      auto res =
          compress(v.data(), &len, uncompressed.data(), uncompressed.size());
      if (res == Z_BUF_ERROR) continue;
      if (res != Z_OK) return {};
      v.resize(len);
      break;
    }
    return v;
  }

  void PrintHex(const V &v, size_t max_n) {
    for (size_t i = 0; i < max_n && i < v.size(); i++) {
      std::cerr << "0x" << std::hex << (unsigned)v[i] << " " << std::dec;
    }
    std::cerr << std::endl;
  }

  V ihdr_;

//   struct Chunk {
//     uint32_t type;
//     V v;
//   };
  std::vector<Chunk> chunks_;
};


#ifdef YOLO_PNG_MUTATOR_DEFINE_LIBFUZZER_CUSTOM_MUTATOR

extern "C" size_t LLVMFuzzerMutate(uint8_t *Data, size_t Size, size_t MaxSize);

#if STANDALONE_TARGET
size_t LLVMFuzzerMutate(uint8_t *Data, size_t Size, size_t MaxSize) {
  assert(false && "LLVMFuzzerMutate should not be called from StandaloneFuzzTargetMain");
  return 0;
}
#endif

extern "C" size_t LLVMFuzzerCustomMutator(uint8_t *Data, size_t Size,
                                          size_t MaxSize, unsigned int Seed) {
  std::string s(reinterpret_cast<const char*>(Data), Size);
  std::stringstream in(s);
  std::stringstream out;
  YoloPngMutator p(in);
  p.Mutate(LLVMFuzzerMutate, Seed);
  p.Serialize(out);
  const auto &str = out.str();
  if (str.size() > MaxSize) return Size;
  memcpy(Data, str.data(), str.size());
  return str.size();
}

extern "C" size_t LLVMFuzzerCustomCrossOver(const uint8_t *Data1, size_t Size1,
                                            const uint8_t *Data2, size_t Size2,
                                            uint8_t *Out, size_t MaxOutSize,
                                            unsigned int Seed) {
  std::stringstream in1(
      std::string(reinterpret_cast<const char *>(Data1), Size1));
  std::stringstream in2(
      std::string(reinterpret_cast<const char *>(Data2), Size2));
  YoloPngMutator p1(in1);
  YoloPngMutator p2(in2);
  p1.CrossOver(p2, Seed);
  std::stringstream out;
  p1.Serialize(out);
  const auto &str = out.str();
  if (str.size() > MaxOutSize) return 0;
  memcpy(Out, str.data(), str.size());
  return str.size();
}

#endif  // YOLO_PNG_MUTATOR_DEFINE_LIBFUZZER_CUSTOM_MUTATOR
