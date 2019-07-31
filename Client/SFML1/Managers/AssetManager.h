#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <map>
#include <ctime>
#include <memory>


using namespace sf;

class AssetManager
{
public:

	~AssetManager();
	
	void loadTexture(const String &file_name);
	void loadSoundBuffer(const String &file_name);
	void loadFont(const String &file_name);

	Texture& getTexture(const String &file_name);
	SoundBuffer& getSoundBuffer(const String &file_name);
	Font& getFont(const String &file_name);

	void selectMusic(int m_id);
	void butMusic();

	SoundBuffer& getRandASound();
	SoundBuffer& getRandDSound();

private:
	std::map<std::string, std::unique_ptr<Texture>> m_Textures;
	std::map<std::string, std::unique_ptr<SoundBuffer>> m_SoundBuffers;
	std::map<std::string, std::unique_ptr<Font>> m_Fonts;

	Music* m_music;
	int c_music;
};

