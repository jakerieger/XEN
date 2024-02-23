#include <assimp/Importer.hpp>
#include <assimp/Exporter.hpp>
#include <assimp/postprocess.h>
#include <stdint.h>
#include <filesystem>
#include <format>
#include <assimp/scene.h>
#include <rapidjson/document.h>

using namespace rapidjson;

constexpr uint32_t g_ProcessFlags =
  aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace |
  aiProcess_JoinIdenticalVertices | aiProcess_SortByPType;

static int Convert(const std::string& file, std::string& outFilename) {
    if (!std::filesystem::exists(file)) {
        printf("      * File does not exist\n");
        return 1;
    }

    const auto outputFile = std::string(file);
    outFilename =
      outputFile.substr(0, outputFile.find_last_of('.')).append(".assbin");

    Assimp::Importer importer;
    Assimp::Exporter exporter;

    const aiScene* scene = importer.ReadFile(file, g_ProcessFlags);
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
        !scene->mRootNode) {
        fprintf(stderr, "ERROR::ASSIMP::%s\n", importer.GetErrorString());
        return 1;
    }
    exporter.Export(scene, "assbin", outputFile);

    return 0;
}

static void ParseManifest(const std::string& json) {
    Document manifest;
    manifest.Parse(json.c_str());

    assert(manifest.IsObject());
    assert(manifest.HasMember("convert"));
    assert(manifest["convert"].HasMember("files"));
    assert(manifest["convert"]["files"].IsArray());

    auto files = manifest["convert"]["files"].GetArray();

    uint32_t converted = 0;
    printf("Converting files:\n");
    for (SizeType i = 0; i < files.Size(); i++) {
        const char* filename = files[i].GetString();
        printf("  - %s\n", filename);

        std::string outFile;
        if (Convert(filename, outFile) != 0) {
            continue;
        }

        printf("  => %s\n", outFile.c_str());
        converted++;
    }

    printf("\nDone! Successfully converted %d file(s)\n", converted);
}

static std::string ReadFile(const char* path);

int main(const int argc, char* argv[]) {
    // Check if a file was passed as an argument first
    if (argc == 2) {
        if (!std::filesystem::exists(argv[1])) {
            fprintf(stderr, "File provided does not exist\n");
            return 1;
        }

        printf("Converting 1 file:\n");
        printf("  - %s\n", argv[1]);
        std::string outFile;
        Convert(argv[1], outFile);
    }

    // If no file was passed, look for manifest.json in program path
    if (!std::filesystem::exists("./manifest.json")) {
        fprintf(stderr, "No file provided and no manifest file found\n");
        return 1;
    }

    auto manifestJson = ReadFile("manifest.json");
    if (manifestJson == "") {
        fprintf(stderr, "Error reading manifest file\n");
    }
    ParseManifest(manifestJson);

    return 0;
}

std::string ReadFile(const char* path) {
    if (FILE* fp = fopen(path, "rb")) {
        std::string contents;
        fseek(fp, 0, SEEK_END);
        contents.resize(ftell(fp));
        rewind(fp);
        fread(&contents[0], 1, contents.size(), fp);
        fclose(fp);
        return contents;
    }

    return "";
}
