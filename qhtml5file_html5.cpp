#include "qhtml5file.h"
#include <qdebug.h>
#include <emscripten.h>
#include <emscripten/html5.h>


std::function<void(char *, size_t, const char *)> g_qtFileDataReadyCallback;
extern "C" EMSCRIPTEN_KEEPALIVE void qt_callFileDataReady(char *content, size_t contentSize, const char *fileName)
{
    if (g_qtFileDataReadyCallback == nullptr)
        return;
    g_qtFileDataReadyCallback(content, contentSize, fileName);
    g_qtFileDataReadyCallback = nullptr;
}

namespace {

void saveFile(const char *contentPointer, size_t contentLength, const char *fileNameHint)
    {
        EM_ASM_({
            // Make the file contents and file name hint accessible to Javascript: convert
            // the char * to a JavaScript string and create a subarray view into the C heap.
            const contentPointer = $0;
            const contentLength = $1;
            const fileNameHint = UTF8ToString($2);
            const fileContent = Module.HEAPU8.subarray(contentPointer, contentPointer + contentLength);
            // Create a hidden download link and click it programatically
            const fileblob = new Blob([fileContent], { type : "application/octet-stream" } );
            var link = document.createElement("a");
            document.body.appendChild(link);
            link.download = fileNameHint;
            link.href = window.URL.createObjectURL(fileblob);
            link.style = "display:none";
            link.click();
            document.body.removeChild(link);
        }, contentPointer, contentLength, fileNameHint);
    }
}

void QHtml5File::save(const QByteArray &content,const  QString &fileNameHint)
{
    // Convert to C types and save
    saveFile(content.constData(), content.size(), fileNameHint.toUtf8().constData());
}
