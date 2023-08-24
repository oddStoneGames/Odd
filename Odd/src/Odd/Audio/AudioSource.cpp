#include "AudioSource.h"
#include "Odd/Core/Log.h"

#include "include/AL/al.h"
#include "sndfile.h"

namespace Odd
{
	Ref<AudioSource> AudioSource::Create(const std::string& file)
	{
		return CreateRef<AudioSource>(file);
	}

	AudioSource::AudioSource(const std::string& file)
	{
        // Load Buffer
        m_BufferHandle = Load(file);

        // Create Source & Set Source's Buffer.
        alGenSources(1, &m_SourceHandle);
        alSourcei(m_SourceHandle, AL_BUFFER, m_BufferHandle);

        m_AudioFilePath = file;
        m_Loaded = true;
	}

	AudioSource::~AudioSource()
	{
		// Free Source & Buffer.
		alDeleteSources(1, &m_SourceHandle);
		alDeleteBuffers(1, &m_BufferHandle);
	}

	void AudioSource::Play()
	{
        alSourcePlay(m_SourceHandle);
	}

	void AudioSource::Pause()
	{
        alSourcePause(m_SourceHandle);
	}

	void AudioSource::Stop()
	{
        alSourceStop(m_SourceHandle);
	}

    bool AudioSource::IsPlaying()
    {
        ALenum state;
        alGetSourcei(m_SourceHandle, AL_SOURCE_STATE, &state);
        return state == AL_PLAYING;
    }

	void AudioSource::SetLoop(bool loop)
	{
		m_Loop = loop;

		alSourcei(m_SourceHandle, AL_LOOPING, loop ? AL_TRUE : AL_FALSE);
	}

	void AudioSource::SetGain(float gain)
	{
		m_Gain = gain;

		alSourcef(m_SourceHandle, AL_GAIN, gain);
	}

	void AudioSource::SetPitch(float pitch)
	{
		m_Pitch = pitch;

		alSourcef(m_SourceHandle, AL_PITCH, pitch);
	}

	void AudioSource::SetPosition(glm::vec3 pos)
	{
		m_Position[0] = pos.x;
		m_Position[1] = pos.y;
		m_Position[2] = pos.z;
		alSourcefv(m_SourceHandle, AL_POSITION, m_Position);
	}

	std::pair<uint32_t, uint32_t> AudioSource::GetLengthMinutesAndSeconds() const
	{
		return { (uint32_t)(m_TotalDuration / 60.0f), (uint32_t)m_TotalDuration % 60 };
	}

	uint32_t AudioSource::Load(const std::string& file)
	{
        ALenum err, format;
        ALuint buffer;
        SNDFILE* sndfile;
        SF_INFO sfinfo;
        short* membuf;
        sf_count_t num_frames;
        ALsizei num_bytes;

        // Open the audio file and check that it's usable.
        sndfile = sf_open(file.c_str(), SFM_READ, &sfinfo);
        if (!sndfile)
        {
            DEBUG_CORE_ERROR("Could not open audio in {0}: {1}", file, sf_strerror(sndfile));
            return 0;
        }
        if (sfinfo.frames < 1 || sfinfo.frames >(sf_count_t)(INT_MAX / sizeof(short)) / sfinfo.channels)
        {
            DEBUG_CORE_ERROR("Bad sample count in {0}", file);
            sf_close(sndfile);
            return 0;
        }

        // Get the sound format, and figure out the OpenAL format
        format = AL_NONE;
        if (sfinfo.channels == 1)
            format = AL_FORMAT_MONO16;
        else if (sfinfo.channels == 2)
            format = AL_FORMAT_STEREO16;

        if (!format)
        {
            DEBUG_CORE_ERROR("Unsupported channel count: {0}", sfinfo.channels);
            sf_close(sndfile);
            return 0;
        }

        // Decode the whole audio file to a buffer.
        auto size = (size_t)(sfinfo.frames * sfinfo.channels) * sizeof(short);
        membuf = (short*)malloc(size);

        num_frames = sf_readf_short(sndfile, membuf, sfinfo.frames);
        if (num_frames < 1)
        {
            free(membuf);
            sf_close(sndfile);
            DEBUG_CORE_ERROR("Failed to read samples in {0}", file);
            return 0;
        }
        num_bytes = (ALsizei)(num_frames * sfinfo.channels) * (ALsizei)sizeof(short);

        // Buffer the audio data into a new buffer object, then free the data and
        // close the file.
        buffer = 0;
        alGenBuffers(1, &buffer);
        alBufferData(buffer, format, membuf, num_bytes, sfinfo.samplerate);

        free(membuf);
        sf_close(sndfile);

        // Check if an error occured, and clean up if so.
        err = alGetError();
        if (err != AL_NO_ERROR)
        {
            DEBUG_CORE_ERROR("OpenAL Error: {0}", alGetString(err));
            if (buffer && alIsBuffer(buffer))
                alDeleteBuffers(1, &buffer);
            return 0;
        }

        return buffer;
	}
}