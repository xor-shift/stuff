#!/bin/bash

[[ -d qoi ]] || exit 1
[[ -d png ]] || exit 2

[[ -d headers ]] || mkdir headers
[[ -d raw ]] || mkdir raw

for f in ./png/*; do
 name=$(basename "$f")
 out_name="raw/${name%.*}.rgba"
 convert -depth 8 "$f" -colorspace sRGB "$out_name" || exit 3
 mv "$out_name" "raw/${name%.*}.data"
done

for f in ./qoi/*; do
  name=$(basename "$f" .qoi)
  xxd -n "${name}_qoi" -i "$f" > "headers/$name.qoi.h" || exit 4
done

for f in ./raw/*; do
  name=$(basename "$f" .data)
	xxd -n "${name}_data" -i "$f" > "headers/$name.data.h" || exit 5
done
