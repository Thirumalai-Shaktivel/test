let
  sources = import ./nix/sources.nix;
  pkgs = import sources.nixpkgs { };
# python=3.7 re2c m2-bison xonsh llvmdev=11.1.0 jupyter xeus xtl nlohmann_json cppzmq jupyter_kernel_test pytest
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
  mkShellNewEnv = pkgs.mkShell.override { stdenv = pkgs.gcc10Stdenv; };
in mkShellNewEnv {
  nativeBuildInputs = [ pkgs.cmake ];
  buildInputs = with pkgs; [
    customPython
    gtest
    bashInteractive
    which
    gcc10Stdenv
    gfortran
    valgrind
    gdb

    llvm_11
    bison_3_5
    libbfd
    re2c
    git
    xonsh
    rapidjson
  ];
}
