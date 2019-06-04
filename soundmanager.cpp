#include <bass.h>

#include "soundmanager.h"

#ifdef Q_OS_LINUX
#include <dlfcn.h>
#else
#include <libloaderapi.h>
#endif

#if 1

#ifdef Q_OS_LINUX
#define LOAD_SYMBOL(module, name) dlsym(module, name)
#else
#define LOAD_SYMBOL(module, name) GetProcAddress(module, name)
#endif

class Module {
public:
	explicit Module(const char *filename) 
#ifdef Q_OS_LINUX
		: module(dlopen(filename, RTLD_LAZY)) {
#else
		: module(LoadLibraryA(filename)) {
#endif
	}

	Module(const Module&) = delete;
	Module& operator=(const Module&) = delete;

	~Module() {
#ifdef Q_OS_LINUX
		dlclose(module);
#else
		FreeLibrary(module);
#endif
	}

#ifdef Q_OS_LINUX
    void* module;
#else
	HMODULE module;
#endif
};

using BASS_InitPfn = decltype(&BASS_Init);
using BASS_FreePfn = decltype(&BASS_Free);
typedef BOOL(*BASS_StreamCreateFilePfn)(BOOL mem, const void *file, QWORD offset, QWORD length, DWORD flags);
using BASS_StreamFreePfn = decltype(&BASS_StreamFree);
using BASS_ChannelSetAttributePfn = decltype(&BASS_ChannelSetAttribute);
using BASS_ChannelPlayPfn = decltype(&BASS_ChannelPlay);
using BASS_ChannelStopPfn = decltype(&BASS_ChannelStop);

class BassLib {
public:
    static BassLib & get() {
        static BassLib lib;
        return lib;
    }

private:
	Module module;

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
    #ifdef Q_OS_LINUX
        : module("./ThirdParty/bass/linux/libbass.so")
    #else
        : module("./ThirdParty/bass/win32/bass.dll")
    #endif
        , BASS_Init((BASS_InitPfn)LOAD_SYMBOL(module.module, "BASS_Init"))
        , BASS_Free((BASS_FreePfn)LOAD_SYMBOL(module.module, "BASS_Free"))
        , BASS_StreamCreateFile((BASS_StreamCreateFilePfn)LOAD_SYMBOL(module.module, "BASS_StreamCreateFile"))
        , BASS_StreamFree((BASS_StreamFreePfn)LOAD_SYMBOL(module.module, "BASS_StreamFree"))
        , BASS_ChannelSetAttribute((BASS_ChannelSetAttributePfn)LOAD_SYMBOL(module.module, "BASS_ChannelSetAttribute"))
        , BASS_ChannelPlay((BASS_ChannelPlayPfn)LOAD_SYMBOL(module.module, "BASS_ChannelPlay"))
        , BASS_ChannelStop((BASS_ChannelStopPfn)LOAD_SYMBOL(module.module, "BASS_ChannelStop")) {
    }
};

class BasInit {
public:
    static BasInit& get() {
        static BasInit instance;
        return instance;
    }

private:
    BasInit() {
        BassLib::get().BASS_Init(-1, 44100, 0, nullptr, nullptr);
    }

    ~BasInit() {
        BassLib::get().BASS_Free();
    }
};

class SoundManager::SoundPlayer {
public:
    explicit SoundPlayer(bool _is_loop)
        : is_loop(_is_loop)
        , stream(0) {	
    }

    ~SoundPlayer() {
        close();
    }

    SoundPlayer(const SoundPlayer &) = delete;
    SoundPlayer& operator=(const SoundPlayer &) = delete;

    void load(const QString &filename) {
        close();
        stream = BassLib::get().BASS_StreamCreateFile(false,
                                                      filename.toLatin1(),
                                                      0,
                                                      0,
                                                      is_loop ? BASS_SAMPLE_LOOP : 0);
    }

    void setMuted(bool status) {
		setVolume(status ? 0 : 100);
    }

    void setVolume(int vol) {
        if (stream != 0) {
            BassLib::get().BASS_ChannelSetAttribute(stream, BASS_ATTRIB_VOL, double(vol) / 100.0);
        }
    }

    void close() {
        if (stream != 0) {
            BassLib::get().BASS_StreamFree(stream);
        }
        stream = 0;
    }

    void play() {
        if (stream != 0) {
            BassLib::get().BASS_ChannelPlay(stream, true);
        }
    }

    void stop() {
        if (stream != 0) {
            BassLib::get().BASS_ChannelStop(stream);
        }
    }

	operator HSTREAM() const {
		return stream;
	}
private:
    bool is_loop;
    HSTREAM stream;
};
#endif

SoundManager::SoundManager(QObject* parent)
    : QObject(parent) {
    BasInit::get();
}

SoundManager::~SoundManager() {
}

void SoundManager::addSound(const QUrl& path, bool is_loop) {
    auto player = new SoundPlayer(is_loop);
    player->load(path.toLocalFile());
    playlist.emplace_back(player);
}

void SoundManager::play(int index) {
    if (playlist.empty()) {
        return;
    }
    playlist[index]->play();
}

void SoundManager::setMuted(int index, bool status) {
    if (playlist.empty()) {
        return;
    }
    playlist[index]->setMuted(status);
}

void SoundManager::setVolume(int index, int vol) {
    if (playlist.empty()) {
        return;
    }
    playlist[index]->setVolume(vol);
}

