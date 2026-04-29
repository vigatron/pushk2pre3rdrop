#pragma once
#include <iostream>
#include <stdexcept>
#include <string>

int parseArgInt(const char *arg, bool &ok);

/*

struct Config {
  std::string cmd; // "s" (split) or "m" (merge)
  std::string input_file;
  std::string rdroptbl_file;
  std::string output_file;
  std::size_t start_offset = 0; // offset in bytes
  std::size_t block_size = 0;   // block size in bytes
  std::size_t count = 0;        // maximum 256
};


class ArgParser {
public:
  static Config parse(int argc, char **argv) {
    if (argc < 2) {
      throw std::runtime_error("Not enough arguments: specify command (s/m)");
    }

    Config cfg;
    cfg.cmd = argv[1];
    if (cfg.cmd != "s" && cfg.cmd != "m") {
      throw std::runtime_error("cmd must be 's' or 'm'");
    }

    for (int i = 2; i < argc; ++i) {
      std::string arg = argv[i];
      if (arg == "--start-offset" && i + 1 < argc) {
        cfg.start_offset = std::stoul(argv[++i]);
      } else if (arg == "--block-size" && i + 1 < argc) {
        cfg.block_size = std::stoul(argv[++i]);
      } else if (arg == "--count" && i + 1 < argc) {
        cfg.count = std::stoul(argv[++i]);
      } else if (arg == "--input" && i + 1 < argc) {
        cfg.input_file = argv[++i];
      } else if (arg == "--output" && i + 1 < argc) {
        cfg.output_file = argv[++i];
      } else if (arg == "--rdroptbl" && i + 1 < argc) {
        cfg.rdroptbl_file = argv[++i];
      } else {
        throw std::runtime_error("Unknown argument: " + arg);
      }
    }

    validate(cfg);
    return cfg;
  }

private:
  static void validate(const Config &cfg) {
    // cmd already validated
    if (cfg.block_size == 0) {
      throw std::runtime_error("block_size must be greater than 0");
    }
    if (cfg.count == 0 || cfg.count > 256) {
      throw std::runtime_error("count must be between 1 and 256");
    }
    if (cfg.input_file.empty()) {
      throw std::runtime_error("input_file must be specified");
    }
    if (cfg.output_file.empty()) {
      throw std::runtime_error("output_file must be specified");
    }
    if (cfg.rdroptbl_file.empty()) {
      throw std::runtime_error("rdroptbl_file must be specified");
    }
  }
};
*/
