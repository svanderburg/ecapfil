with import <nixpkgs> {};

stdenv.mkDerivation {
  name = "ecapfil";
  src = ./.;
  buildPhase = "make";
  installFlags = "PREFIX=$(out)";
  buildInputs = [ pkg-config SDL2.dev SDL2_image ];
  dontStrip = true;
}
