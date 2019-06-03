#include <dlfcn.h>
#include <bass.h>

#include "soundmanager.h"

#if 1
#if 0
template <typename FunctionType>
class LibraryLoader {
public:
    LibraryLoader(const char *name, void *module) {
        fun = reinterpret_cast<FunctionType*>(dlsym(module, name));
    }

    template <typename ...Args>
    typename std::result_of<FunctionType(Args...)>::type operator()(Args... args) {
        return (*fun)(args...);
    }
private:
    FunctionType *fun;
};
#endif

using BASS_InitPfn = decltype(&BASS_Init);
using BASS_FreePfn = decltype(&BASS_Free);
using BASS_StreamCreateFilePfn = decltype(&BASS_StreamCreateFile);
using BASS_StreamFreePfn = decltype(&BASS_StreamFree);
using BASS_ChannelSetAttributePfn = decltype(&BASS_ChannelSetAttribute);
using BASS_ChannelPlayPfn = decltype(&BASS_ChannelPlay);
using BASS_ChannelStopPfn = decltype(&BASS_ChannelStop);

class BassLib {
public:
    ~BassLib() {
        dlclose(module);
    }

    static BassLib & Get() {
        static BassLib lib;
        return lib;
    }

private:
    void *module;

public:
    BASS_InitPfn BASS_Init;
    BASS_FreePfn BASS_Free;
    BASS_StreamCreateFilePfn BASS_StreamCreateFile;
    BASS_StreamFreePfn BASS_StreamFree;
    BASS_ChannelSetAttributePfn BASS_ChannelSetAttribute;
    BASS_ChannelPlayPfn BASS_ChannelPlay;
    BASS_ChannelStopPfn BASS_ChannelStop;

private:
    BassLib()
        : module(dlopen("./ThirdParty/bass/linux/libbass.so", RTLD_LAZY))
        , BASS_Init((BASS_InitPfn)dlsym(module, "BASS_Init"))
        , BASS_Free((BASS_FreePfn)dlsym(module, "BASS_Free"))
        , BASS_StreamCreateFile((BASS_StreamCreateFilePfn)dlsym(module, "BASS_StreamCreateFile"))
        , BASS_StreamFree((BASS_StreamFreePfn)dlsym(module, "BASS_StreamFree"))
        , BASS_ChannelSetAttribute((BASS_ChannelSetAttributePfn)dlsym(module, "BASS_ChannelSetAttribute"))
        , BASS_ChannelPlay((BASS_ChannelPlayPfn)dlsym(module, "BASS_ChannelPlay"))
        , BASS_ChannelStop((BASS_ChannelStopPfn)dlsym(module, "BASS_ChannelStop")) {
    }
};

class BasInit {
public:
    static BasInit& Get() {
        static BasInit instance;
        return instance;
    }

private:
    BasInit() {
        BassLib::Get().BASS_Init(-1, 44100, 0, nullptr, nullptr);
    }

    ~BasInit() {
        BassLib::Get().BASS_Free();
    }
};

class SoundManager::SoundPlayer {
public:
    explicit SoundPlayer(bool _is_loop)
        : is_loop(_is_loop)
        , stream(0) {
        BasInit::Get();
    }

    ~SoundPlayer() {
        close();
    }

    SoundPlayer(const SoundPlayer &) = delete;
    SoundPlayer& operator=(const SoundPlayer &) = delete;

    void load(const QString &filename) {
        close();
        stream = BassLib::Get().BASS_StreamCreateFile(false,
                                                      filename.toLatin1(),
                                                      0,
                                                      0,
                                                      is_loop ? BASS_SAMPLE_LOOP : 0);
    }

    void setMuted(bool status) {
        if (stream != 0) {
            BassLib::Get().BASS_ChannelSetAttribute(stream, BASS_ATTRIB_VOL, status ? 0 : 100);
        }
    }

    void close() {
        if (stream != 0) {
            BassLib::Get().BASS_StreamFree(stream);
        }
        stream = 0;
    }

    void play() {
        if (stream != 0) {
            BassLib::Get().BASS_ChannelPlay(stream, true);
        }
    }

    void stop() {
        if (stream != 0) {
            BassLib::Get().BASS_ChannelStop(stream);
        }
    }
private:
    bool is_loop;
    HSTREAM stream;
};
#endif

SoundManager::SoundManager(QObject* parent)
    : QObject(parent) {
}

SoundManager::~SoundManager() {
}

void SoundManager::addSound(const QUrl& path, bool is_loop) {
    auto player = new SoundPlayer(is_loop);
    player->load(path.toLocalFile());
    playlist.emplace_back(player);
}

void SoundManager::play(int index) {
    playlist[index]->play();
}

void SoundManager::setMuted(int index, bool status) {
    playlist[index]->setMuted(status);
}
