{pkgs ? import <nixpkgs> {}}:
pkgs.mkShell {
  buildInputs = with pkgs; [
    meson
    ninja

    pkg-config

    clang-tools
    clang

    gtk3
    dbus
    curl
    glib
    sdbus-cpp
    nlohmann_json
    systemdLibs
  ];
  # nativeBuildInputs = with pkgs; [];

  shellHook = ''
    # export PATH=$PATH:
  '';
}
