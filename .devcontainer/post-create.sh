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

# Install prerequisite packages.
sudo apt-get -y update
sudo apt-get -y dist-upgrade

sudo chown vscode:vscode /workspaces

# Clone `prerequisites`.
pushd /workspaces
git clone https://github.com/tomohxx/shanten-number
pushd shanten-number
tar -xzf data.tar.gz
popd
popd
