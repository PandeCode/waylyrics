# Waylyrics - A Spotify Lyrics Module for Waybar

Successor to [lyrify](https://github.com/pandecode/lyrify)

Waylyrics is a Waybar module that retrieves the currently playing lyrics from Spotify using CFFI. It integrates with Waybar via the CFFI interface and displays synchronized or plain lyrics in real-time.

## How It Works

1. **D-Bus Communication**:

   - The module communicates with Spotify via D-Bus (`sdbus-c++`) to fetch metadata, including the song title, artist, and playback position.

2. **Fetching Lyrics**:

   - It queries the [lrclib.net](https://lrclib.net/) API to fetch lyrics, caching results to `~/.cache/waylyrics` for efficiency.
   - If synchronized lyrics are available, it determines the current line based on the playback position.
   - If only plain lyrics exist, it approximates the current line.

3. **Integration with Waybar**:
   - The module is built as a shared library (`waylyrics.so`) and loaded via Waybar's CFFI system.
   - It runs a separate thread to update the lyrics in real time.

## Dependencies

To build and run this module, install the following:

- **Build dependencies**:

  - `g++` (C++ compiler)
  - `meson` & `ninja` (build system)
  - `pkg-config`

- **Libraries**:
  - `gtk-3`
  - `sdbus-c++`
  - `curl`
  - `nlohmann-json`

## Building & Installing

CI builds are up. (check tags/releases)

Run the following:

```sh
just # setup meson
just build-waylyrics # builds the shared library
```

This will compile `waylyrics.so` and place it in the current working directory.

## Configuration

Add the following to your Waybar configuration:

```json
"cffi/lyrics": {
    "module_path": "/<path to library>/waylyrics.so"
}
```

Restart Waybar, and you should see lyrics appear when playing music on Spotify.

## Usage of the `justfile`

A `justfile` (Just build system) is included to simplify build commands:

- **`just`** → Compiles the module.
- **`just build-waylyrics`** → Installs `waylyrics.so` to the appropriate directory.
- **`just clean`** → Removes compiled artifacts.

## License

MIT License. Contributions welcome!
