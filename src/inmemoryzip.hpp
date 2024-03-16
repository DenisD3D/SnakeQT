#ifndef IN_MEMORY_ZIP_FILE_HANDLER_H
#define IN_MEMORY_ZIP_FILE_HANDLER_H

#include <QDebug>

#include "unzip.h"
#include "zip.h"


class QByteArray;
class QString;
class QTextStream;


/**
 * Handle writing contents of a ZIP file
 * Used to write map data and resources to .skm files
 */
class InMemoryZipFileWriter {
    zipFile file;

public:
    explicit InMemoryZipFileWriter(const QString &fileName);

    ~InMemoryZipFileWriter();

    bool addFileToZip(const QString &fileName, const QByteArray &data) const;

    /**
     * @brief isValid
     * @return true if the ZIP file was opened successfully
     */
    bool isValid() const { return file != nullptr; }
};

/**
 * Handle reading contents of a ZIP file without extracting it to disk
 * Used to read map data and resources from .skm files
 * Based on https://asmaloney.com/2011/12/code/in-memory-zip-file-access-using-qt/
 */
class InMemoryZipFileReader {
        unzFile file;

public:
    explicit InMemoryZipFileReader(const QString &inFileName);

    ~InMemoryZipFileReader();

    /**
     * @brief isValid
     * @return true if the ZIP file was opened successfully
     */
    bool isValid() const { return file != nullptr; }

    /**
     * Extract a file from the ZIP file and put the contents into a QByteArray
     * @param fileName name of the file in the zip to extract
     * @param data QByteArray to put the file contents into
     * @return error code
     */
    bool extractFile(const QString &fileName, QByteArray &data) const;
};
#endif
