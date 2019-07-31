#include "AssetManager.h"



AssetManager::~AssetManager()
{
	if (m_music != nullptr) { delete m_music; }
}

void AssetManager::loadTexture(const String & file_name)
{
	std::unique_ptr<Texture> texture(new Texture());
	Image image; image.loadFromFile(file_name);
	image.createMaskFromColor(Color(255, 255, 255));
	texture->loadFromImage(image);

	this->m_Textures.insert(std::make_pair(file_name, std::move(texture)));
}

void AssetManager::loadSoundBuffer(const String & file_name)
{
	std::unique_ptr<SoundBuffer> sound_buffer(new SoundBuffer());
	sound_buffer->loadFromFile(file_name);
	this->m_SoundBuffers.insert(std::make_pair(file_name, std::move(sound_buffer)));
}

void AssetManager::loadFont(const String & file_name)
{
	std::unique_ptr<Font> font(new Font());
	font->loadFromFile(file_name);

	this->m_Fonts.insert(std::make_pair(file_name, std::move(font)));
}

Texture & AssetManager::getTexture(const String & file_name)
{
	auto found = this->m_Textures.find(file_name);
	return *found->second;
}

SoundBuffer & AssetManager::getSoundBuffer(const String & file_name)
{
	auto found = this->m_SoundBuffers.find(file_name);
	return *found->second;
}

Font & AssetManager::getFont(const String & file_name)
{
	auto found = this->m_Fonts.find(file_name);
	return *found->second;
}

void AssetManager::selectMusic(int m_id)
{
	if(m_id != c_music)
	{
		if (m_music != nullptr) { m_music->stop(); delete m_music; }
		m_music = new Music;
		if (m_music->openFromFile("../Coursework/Client/SFML1/res/w.res/w" + std::to_string(m_id) + ".music.ogg")) { m_music->play(); }
		c_music = m_id;
	}
}

void AssetManager::butMusic()
{
	if (m_music->getStatus() == Music::Playing) { m_music->stop(); }
	else { m_music->play(); }
}

SoundBuffer & AssetManager::getRandASound()
{
	srand(time(0));
	int i = 1 + rand() % 4;

	std::string rand_sound = "../Coursework/Client/SFML1/res/main.res/sounds/out" + std::to_string(i) + ".ogg";

	return this->getSoundBuffer(rand_sound);
}

SoundBuffer & AssetManager::getRandDSound()
{
	srand(time(0));
	int i = 1 + rand() % 2;

	std::string rand_sound = "../Coursework/Client/SFML1/res/main.res/sounds/in" + std::to_string(i) + ".ogg";

	return this->getSoundBuffer(rand_sound);
}
