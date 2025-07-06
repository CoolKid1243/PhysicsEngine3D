#!/bin/bash

BUILD_DIR="build"
BINARY_NAME="PhysicsEngine3D"

if [ -x "$BUILD_DIR/$BINARY_NAME" ]; then
  pushd "$BUILD_DIR"
  ./"$BINARY_NAME"
  popd
else
  echo "Binary not found. Please build the project first."
  exit 1
fi
