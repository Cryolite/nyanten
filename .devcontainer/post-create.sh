#!/usr/bin/env bash

set -euxo pipefail

PS4='+${BASH_SOURCE[0]}:$LINENO: '
if [[ -t 1 ]] && type -t tput >/dev/null; then
  if (( "$(tput colors)" == 256 )); then
    PS4='$(tput setaf 10)'$PS4'$(tput sgr0)'
  else
    PS4='$(tput setaf 2)'$PS4'$(tput sgr0)'
  fi
fi

sudo apt-get -y update
sudo apt-get -y dist-upgrade
sudo apt-get clean
sudo rm -rf /var/lib/apt/lists/*

sudo chown vscode:vscode /workspaces

pushd /workspaces
git clone https://github.com/Cryolite/prerequisites
popd

/workspaces/prerequisites/gcc/install --debug --prefix "$HOME/.local"
echo 'export C_INCLUDE_PATH="$HOME/.local/include${C_INCLUDE_PATH:+:$C_INCLUDE_PATH}"' >> "$HOME/.bashrc"
echo 'export C_INCLUDE_PATH="$HOME/.local/include${C_INCLUDE_PATH:+:$C_INCLUDE_PATH}"' >> "$HOME/.profile"
export C_INCLUDE_PATH="$HOME/.local/include${C_INCLUDE_PATH:+:$C_INCLUDE_PATH}"
echo 'export CPLUS_INCLUDE_PATH="$HOME/.local/include${CPLUS_INCLUDE_PATH:+:$CPLUS_INCLUDE_PATH}"' >> "$HOME/.bashrc"
echo 'export CPLUS_INCLUDE_PATH="$HOME/.local/include${CPLUS_INCLUDE_PATH:+:$CPLUS_INCLUDE_PATH}"' >> "$HOME/.profile"
export CPLUS_INCLUDE_PATH="$HOME/.local/include${CPLUS_INCLUDE_PATH:+:$CPLUS_INCLUDE_PATH}"
echo 'export LIBRARY_PATH="$HOME/.local/lib64:$HOME/.local/lib${LIBRARY_PATH:+:$LIBRARY_PATH}"' >> "$HOME/.bashrc"
echo 'export LIBRARY_PATH="$HOME/.local/lib64:$HOME/.local/lib${LIBRARY_PATH:+:$LIBRARY_PATH}"' >> "$HOME/.profile"
export LIBRARY_PATH="$HOME/.local/lib64:$HOME/.local/lib${LIBRARY_PATH:+:$LIBRARY_PATH}"
echo 'export LD_LIBRARY_PATH="$HOME/.local/lib64:$HOME/.local/lib${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}"' >> "$HOME/.bashrc"
echo 'export LD_LIBRARY_PATH="$HOME/.local/lib64:$HOME/.local/lib${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}"' >> "$HOME/.profile"
export LD_LIBRARY_PATH="$HOME/.local/lib64:$HOME/.local/lib${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}"
echo 'export CC="$HOME/.local/bin/gcc"' >> "$HOME/.bashrc"
echo 'export CC="$HOME/.local/bin/gcc"' >> "$HOME/.profile"
export CC="$HOME/.local/bin/gcc"
echo 'export CXX="$HOME/.local/bin/g++"' >> "$HOME/.bashrc"
echo 'export CXX="$HOME/.local/bin/g++"' >> "$HOME/.profile"
export CXX="$HOME/.local/bin/g++"

/workspaces/prerequisites/cmake/install --debug --prefix "$HOME/.local"

# Clone `shanten-number`.
pushd /workspaces
git clone https://github.com/tomohxx/shanten-number
pushd shanten-number
tar -xzf data.tar.gz
popd
popd
