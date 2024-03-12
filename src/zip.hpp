#ifndef IN_MEMORY_ZIP_FILE_HANDLER_H
#define IN_MEMORY_ZIP_FILE_HANDLER_H

#include "unzip.h"


class QByteArray;
class QString;
class QTextStream;

/**
 * Handle reading contents of a ZIP file without extracting it to disk
 * Used to read map data and resources from .skm files
 * Based on https://asmaloney.com/2011/12/code/in-memory-zip-file-access-using-qt/
 */
class InMemoryZipFileHandler {
public:
    /**
     * @brief Enum for case sensitivity.
     * @details See unzStringFileNameCompare() in unzip.c for more details.
     */
    enum eCaseSensitivity {
        CASE_OS_DEFAULT = 0,
        CASE_SENSITIVE,
        CASE_INSENSITIVE
    };

    /**
     * Wrap the minizip-ng error defines from unzip.h
     */
    enum eErrCode {
        NO_ERR = UNZ_OK,

        ERR_END_OF_LIST_OF_FILE = UNZ_END_OF_LIST_OF_FILE,
        ERR_ERRNO = UNZ_ERRNO,
        ERR_EOF = UNZ_EOF,
        ERR_PARAMERROR = UNZ_PARAMERROR,
        ERR_BADZIPFILE = UNZ_BADZIPFILE,
        ERR_INTERNALERROR = UNZ_INTERNALERROR,
        ERR_CRCERROR = UNZ_CRCERROR,

        ERR_FILE_NOT_FOUND_IN_ZIP = -1000
    };

public:
    explicit InMemoryZipFileHandler(const QString &inFileName);

    ~InMemoryZipFileHandler();

    /**
     * @brief isValid
     * @return true if the ZIP file was opened successfully
     */
    bool isValid() const { return mFile != nullptr; }

    /**
     * Extract a file from the ZIP file and put the contents into a QByteArray
     * @param inFileName name of the file in the zip to extract
     * @param outData QByteArray to put the file contents into
     * @param inCaseSensitive whether to match the file name case sensitively
     * @return error code
     */
    eErrCode extractFile(const QString &inFileName, QByteArray &outData,
                         eCaseSensitivity inCaseSensitive = CASE_OS_DEFAULT) const;

    // List the contents of the ZIP file
    //    Modified version of the do_list() function in miniunz.c
    /**
     * Utility function to print the contents of the ZIP file
     * @return error code
     */
    eErrCode listFiles() const;

private:
    static void sOutputListLine(QTextStream &out, const QStringList &inStrings);

    unzFile mFile;
};
#endif
