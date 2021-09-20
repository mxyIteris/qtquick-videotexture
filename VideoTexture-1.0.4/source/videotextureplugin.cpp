#include "videotextureplugin.h"
#include "videotextureitem.h"

void VideoTexturePlugin::registerTypes(const char *uri)
{
    Q_ASSERT(uri == QLatin1String("VideoTexture"));
    qmlRegisterType<VideoTextureItem>(uri, 1, 0, "VideoTexture");
}