{
  description = "A development environment with the necessary dependencies";

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs/nixos-23.11";
  };

  outputs = {
    self,
    nixpkgs,
  }: let
    system = "x86_64-linux";
    pkgs = import nixpkgs {inherit system;};
  in {
    devShells = {
      x86_64-linux.default = pkgs.mkShell {
        buildInputs = with pkgs; [
          meson
          just
          ninja
          pkg-config
          gtk3
          dbus
          curl
          glib
          sdbus-cpp
          nlohmann_json
          systemdLibs
        ];

        shellHook = ''
          # export PATH=$PATH:
        '';
      };
    };
  };
}
