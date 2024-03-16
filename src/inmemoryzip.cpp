#include "inmemoryzip.hpp"


InMemoryZipFileWriter::InMemoryZipFileWriter(const QString &fileName) {
    file = zipOpen(qPrintable(fileName), APPEND_STATUS_CREATE);
    if (file == nullptr) {
        qDebug() << "ERROR: InMemoryZipFileWriter: could not open or create file" << fileName;
    }
}

InMemoryZipFileWriter::~InMemoryZipFileWriter() {
    zipClose(file, nullptr);
}

bool InMemoryZipFileWriter::addFileToZip(const QString &fileName, const QByteArray &data) const {
    if (!isValid()) {
        qDebug() << "ERROR: InMemoryZipFileWriter: zip file is not open";
        return false;
    }

    constexpr zip_fileinfo zipInfo = {};
    int err = zipOpenNewFileInZip(file, qPrintable(fileName), &zipInfo,
                                     nullptr, 0, nullptr, 0, nullptr,
                                     Z_DEFLATED, -1);

    if (err != ZIP_OK) {
        qDebug() << "ERROR: InMemoryZipFileWriter: could not open file in zip" << fileName;
        return false;
    }

    qDebug() << "InMemoryZipFileWriter: inserting" << fileName << "buffer size" << data.size();

    err = zipWriteInFileInZip(file, data.data(), data.size());
    if (err < 0) {
        zipCloseFileInZip(file);
        qDebug() << "ERROR: InMemoryZipFileWriter: could not write file in zip" << fileName;
        return false;
    }

    err = zipCloseFileInZip(file);
    if (err != ZIP_OK) {
        qDebug() << "ERROR: InMemoryZipFileWriter: could not close file in zip" << fileName;
        return false;
    }
    return true;
}


InMemoryZipFileReader::InMemoryZipFileReader(const QString &fileName) {
    file = unzOpen64(qPrintable(fileName));
    if (file == nullptr) {
        qDebug() << "ERROR: InMemoryZipFileReader: could not open file" << fileName;
    }
}

InMemoryZipFileReader::~InMemoryZipFileReader() {
    unzClose(file);
}

bool InMemoryZipFileReader::extractFile(const QString &fileName, QByteArray &data) const {
    if (!isValid()) {
        qDebug() << "ERROR: InMemoryZipFileReader: zip file is not open";
        return false;
    }

    if (unzLocateFile(file, qPrintable(fileName), 0) != UNZ_OK) {
        qDebug() << "ERROR: InMemoryZipFileReader: file not found in the zipfile: " << fileName;
        return false;
    }

    unz_file_info64 file_info;

    if (unzGetCurrentFileInfo64(file, &file_info, nullptr, 0, nullptr, 0, nullptr, 0) != UNZ_OK) {
        qDebug() << "ERROR: InMemoryZipFileReader: could not read file info: " << fileName;
        return false;
    }

    if (unzOpenCurrentFile(file) != UNZ_OK) {
        qDebug() << "ERROR: InMemoryZipFileReader: could not open in zip file: " << fileName;
        return false;
    }

    data.fill(0, static_cast<int>(file_info.uncompressed_size + 1));

    qDebug() << "InMemoryZipFileReader: extracting" << fileName << "buffer size" << data.size();

    const int read = unzReadCurrentFile(file, data.data(), data.size());

    unzCloseCurrentFile(file);

    if (read < 0) {
        qDebug() << "ERROR: InMemoryZipFileReader: could not read file in zip" << fileName;
        return false;
    }

    return true;
}