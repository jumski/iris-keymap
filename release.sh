#!/bin/bash
set -e

# Check for uncommitted changes
if ! git diff-index --quiet HEAD --; then
    echo "There are uncommitted changes. Please commit or stash them before creating a release."
    exit 1
fi

# Check for unpushed changes
if ! git diff --quiet origin/$(git branch --show-current); then
    echo "There are unpushed changes. Please push them before creating a release."
    exit 1
fi

latest_commit_title=$(git log --format=%B -n 1 HEAD | head -n 1)
prev_release=$(gh release view --json tagName | jq -r '.tagName')
version=$(echo "$prev_release" | awk -F. -v OFS=. '{$3++; print}')
qmk_firmware_path="$HOME/qmk_firmware/.build/keebio_iris_rev6_jumski.*"

echo "The following command will be executed:"
echo
echo "gh release create $version --title \"$latest_commit_title\" $qmk_firmware_path"
echo

read -p "Do you want to proceed with this release? [y/N] " confirm

if [[ $confirm == [yY] || $confirm == [yY][eE][sS] ]]; then
    gh release create "$version" --title "$latest_commit_title" "$qmk_firmware_path"
else
    echo "Release creation cancelled by user."
fi
