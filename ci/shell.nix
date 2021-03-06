{ clangSupport ? true }:
let
  sources = import ./nix/sources.nix;
  pkgs = import sources.nixpkgs { };
  mach-nix = import (builtins.fetchGit {
    url = "https://github.com/DavHau/mach-nix/";
    ref = "refs/tags/3.1.1";
  }) {
    pkgs = pkgs;
    python = "python37";
  };
  customPython = mach-nix.mkPython rec {
    requirements = ''
      pytest
      toml
    '';
  };
  llvmPkgs = pkgs.buildPackages.llvmPackages_11;
  myStdenv = if clangSupport then llvmPkgs.stdenv else pkgs.gcc10Stdenv;
  mkShellNewEnv = pkgs.mkShell.override { stdenv = myStdenv; };
in mkShellNewEnv {
  nativeBuildInputs = [ pkgs.cmake ];
  buildInputs = with pkgs; [
    customPython
    bashInteractive
    which
    gfortran
    valgrind
    gdb
    fmt
    llvm_11
    lld_11
    llvmPkgs.bintools
    bison_3_5
    zlib
    libbfd
    re2c
    git
    xonsh
    rapidjson
  ];
}
