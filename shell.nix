{ pkgs ? import <nixpkgs> {} }:

pkgs.mkShell {
  nativeBuildInputs = with pkgs; [
    cmake
    pkg-config
    wayland-scanner  # This fixes your specific error
  ];

  buildInputs = with pkgs; [
    wayland
    wayland-protocols
    libxkbcommon
    libGL
    xorg.libX11
    xorg.libXrandr
    xorg.libXinerama
    xorg.libXcursor
    xorg.libXi
  ];
}
