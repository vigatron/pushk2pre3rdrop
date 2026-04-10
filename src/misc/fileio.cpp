#include "fileio.hpp"

//-----------------------------------------------------------
bool fileExists(const std::string &path) {
  try {
    return std::filesystem::exists(path) &&
           std::filesystem::is_regular_file(path);
  } catch (const std::filesystem::filesystem_error &e) {
    std::cerr << "Ошибка доступа: " << e.what() << '\n';
    return false;
  }
}

//-----------------------------------------------------------
std::vector<uint8_t> load_binary_file(const std::string &filename) {
  // 1. Получаем размер файла с помощью C++17 filesystem
  std::filesystem::path p{filename};
  auto fileSize = std::filesystem::file_size(p);

  // 2. Открываем файл в бинарном режиме
  std::ifstream file(filename, std::ios::binary);

  // 3. Резервируем память (конструктор сразу выделяет fileSize байт)
  std::vector<uint8_t> buffer(fileSize);

  // 4. Читаем данные напрямую в буфер вектора
  // Требуется reinterpret_cast, так как ifstream::read ожидает char*
  if (!file.read(reinterpret_cast<char *>(buffer.data()), fileSize)) {
    return {}; // Обработка ошибки чтения
  }

  return buffer;
}
