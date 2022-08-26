#!/bin/bash

install_command_dev_tool() {
    sudo apt update
    sudo apt install -y build-essential tree git vim make zsh
}

setup_shell() {
    # create password
    echo "Please type new password"
    sudo passwd "$USER"

    # make zsh the default shell
    # but you should also change vs code with
    # "terminal.integrated.shell.linux": "/usr/bin/zsh"
    chsh -s "$(which zsh)" "$USER"

    # install oh-my-zsh
    sh -c "$(curl -fsSL https://raw.github.com/ohmyzsh/ohmyzsh/master/tools/install.sh)"

    # change theme of zsh to avit
    sed -i 's/ZSH_THEME="robbyrussell"/ZSH_THEME="avit"/' ~/.zshrc

    # set up git editor, email and username
    git config --global core.editor "vim"
    git config --global user.email "zijiewu@microsoft.com"
    git config --global user.name "Zijie Wu"
}

install_homebrew() {
    /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
}

install_github_cli_tool() {
    brew update
    brew install gh
}

main() {
    install_command_dev_tool
    setup_shell
    install_homebrew
    install_github_cli_tool
}

# Script starts here
main
