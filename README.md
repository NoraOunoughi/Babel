# Babel

Le projet consiste en une application de voix sur IP à architecture client/serveur, similaire à Skype ou TeamSpeak.

## Usage

pip install conan

conan remote add bincrafters \
https://bincrafters.jfrog.io/artifactory/api/conan/public-conan

conan config set general.revisions_enabled=1

mkdir build && cd build && conan install ../ --build=missing && cmake ../ -G "Unix Makefiles" && cmake –build .
