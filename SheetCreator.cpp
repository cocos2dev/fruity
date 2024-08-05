// made by cocos
// use it like you want to use it
// just give credit and dont sell this
// shoutout to chatgpt

#include "SheetCreator.h"
#include <iostream>
#include <filesystem>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <stb_image.h>
#include <stb_image_write.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

namespace fs = std::filesystem;

struct Sprite {
    int x, y, width, height;
    std::string name;
};

int exportSheet(const std::string& name) {
    std::vector<std::string> files;
    std::vector<Sprite> sprites;

    // search for png
    for (const auto& entry : fs::directory_iterator(name)) {
        if (entry.path().extension() == ".png") {
            files.push_back(entry.path().string());
        }
    }

    if (files.size() < 2) {
        std::cerr << "Not enough PNG files to create a spritesheet!" << std::endl;
        return 1;
    }

    std::sort(files.begin(), files.end());

    int spriteWidth = 0;
    int spriteHeight = 0;
    std::vector<uint8_t> allImages;

    // load all images and calculate size or smth
    for (const auto& file : files) {
        int width, height, channels;
        uint8_t* imageData = stbi_load(file.c_str(), &width, &height, &channels, 4);
        if (imageData == nullptr) {
            std::cerr << "Failed to load image: " << file << std::endl;
            return 1;
        }

        spriteWidth = std::max(spriteWidth, width);
        spriteHeight += height;
        allImages.insert(allImages.end(), imageData, imageData + width * height * 4);
        stbi_image_free(imageData);

        sprites.push_back({0, spriteHeight - height, width, height, fs::path(file).stem().string()});
    }

    // create spritesheet image
    std::vector<uint8_t> spritesheet(spriteWidth * spriteHeight * 4, 0);
    int yOffset = 0;
    for (const auto& sprite : sprites) {
        for (int y = 0; y < sprite.height; ++y) {
            for (int x = 0; x < sprite.width; ++x) {
                int srcIndex = (y * sprite.width + x) * 4;
                int destIndex = ((y + yOffset) * spriteWidth + x) * 4;
                spritesheet[destIndex] = allImages[srcIndex];
                spritesheet[destIndex + 1] = allImages[srcIndex + 1];
                spritesheet[destIndex + 2] = allImages[srcIndex + 2];
                spritesheet[destIndex + 3] = allImages[srcIndex + 3];
            }
        }
        yOffset += sprite.height;
    }

    // making it a png
    std::string sheetPath = "./"+ name + "Sheet.png";
    if (!stbi_write_png(sheetPath.c_str(), spriteWidth, spriteHeight, 4, spritesheet.data(), spriteWidth * 4)) {
        std::cerr << "Failed to write spritesheet image." << std::endl;
        return 1;
    }

    // plist file
    std::string plistPath = "./"+ name + "Sheet.plist";
    std::ofstream plistFile(plistPath);
    if (!plistFile.is_open()) {
        std::cerr << "Failed to create plist file." << std::endl;
        return 1;
    }

    plistFile << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
    plistFile << "<plist version=\"1.0\">\n";
    plistFile << "<dict>\n";
    plistFile << "  <key>frames</key>\n";
    plistFile << "  <dict>\n";

    for (const auto& sprite : sprites) {
        plistFile << "    <key>" << sprite.name << "</key>\n";
        plistFile << "    <dict>\n";
        plistFile << "      <key>frame</key>\n";
        plistFile << "      <string>{{" << sprite.x << "," << sprite.y << "},{" << sprite.width << "," << sprite.height << "}}</string>\n";
        plistFile << "      <key>offset</key>\n";
        plistFile << "      <string>{0,0}</string>\n";
        plistFile << "      <key>rotated</key>\n";
        plistFile << "      <false/>\n";
        plistFile << "      <key>sourceColorRect</key>\n";
        plistFile << "      <string>{{0,0},{" << sprite.width << "," << sprite.height << "}}</string>\n";
        plistFile << "      <key>sourceSize</key>\n";
        plistFile << "      <string>{" << sprite.width << "," << sprite.height << "}</string>\n";
        plistFile << "    </dict>\n";
    }

    plistFile << "  </dict>\n";
    plistFile << "  <key>metadata</key>\n";
    plistFile << "  <dict>\n";
    plistFile << "    <key>format</key>\n";
    plistFile << "    <integer>2</integer>\n";
    plistFile << "    <key>size</key>\n";
    plistFile << "    <string>{" << spriteWidth << "," << spriteHeight << "}</string>\n";
    plistFile << "  </dict>\n";
    plistFile << "</dict>\n";
    plistFile << "</plist>\n";
    plistFile.close();

    return 0;
}
