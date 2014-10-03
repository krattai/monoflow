// Copyright © 2013 Galvanized Logic Inc.
// Use is governed by a FreeBSD license found in the LICENSE file.

package main

import (
	"log"
	"time"
	"vu/audio/al"
	"vu/data"
)

// au demonstrates basic audio library (vu/audio/al) capabilities.
// It checks that OpenAL is installed and the bindings are working
// by loading and playing a sound.
func au() {
	al.Init() // map the bindings to the OpenAL dynamic library.

	// open the default device.
	if dev := al.OpenDevice(""); dev != nil {
		defer al.CloseDevice(dev)

		// create a context on the device.
		if ctx := al.CreateContext(dev, nil); ctx != nil {
			defer al.MakeContextCurrent(nil)
			defer al.DestroyContext(ctx)
			al.MakeContextCurrent(ctx)

			// create buffers and sources
			var buffer, source uint32
			al.GenBuffers(1, &buffer)
			al.GenSources(1, &source)

			// read in the audio data.
			sound := &data.Sound{}
			loader := data.NewLoader()
			loader.Load("bloop", &sound)
			if sound == nil {
				log.Printf("au: error loading audio file", "bloop")
				return
			}

			// copy the audio data into the buffer
			tag := &autag{}
			format := tag.audioFormat(sound)
			if format < 0 {
				log.Printf("au: error recognizing audio format")
				return
			}
			al.BufferData(buffer, int32(format), al.Pointer(&(sound.AudioData[0])), int32(sound.DataSize), int32(sound.Frequency))

			// attach the source to a buffer.
			al.Sourcei(source, al.BUFFER, int32(buffer))

			// check for any audio library errors that have happened up to this point.
			if err := al.GetError(); err != 0 {
				log.Printf("au: OpenAL error ", err)
				return
			}

			// Start playback and give enough time for the playback to finish.
			al.SourcePlay(source)
			time.Sleep(500 * time.Millisecond)
			return
		}
		log.Printf("au: error, failed to get a context")
	}
	log.Printf("au: error, failed to get a device")
}

// Globally unique "tag" for this example.
type autag struct{}

// audioFormat figures out which of the OpenAL formats to use based on the
// WAVE file information.
func (a *autag) audioFormat(s *data.Sound) int32 {
	format := int32(-1)
	if s.Channels == 1 && s.SampleBits == 8 {
		format = al.FORMAT_MONO8
	} else if s.Channels == 1 && s.SampleBits == 16 {
		format = al.FORMAT_MONO16
	} else if s.Channels == 2 && s.SampleBits == 8 {
		format = al.FORMAT_STEREO8
	} else if s.Channels == 2 && s.SampleBits == 16 {
		format = al.FORMAT_STEREO16
	}
	return format
}
