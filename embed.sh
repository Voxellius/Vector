#!/bin/bash

OUTPUT_FILE="$1"
SECTION_NAME="$2"
INPUT_FILE="$3"

mkdir -p "$OUTPUT_FILE"
rm -rf "$OUTPUT_FILE"

{
    printf "    .section %s\n" "$SECTION_NAME"
    printf "    .type %s, @object\n" "$SECTION_NAME"
    printf "    .align 4\n"
    printf "    .incbin \"%s\"\n" "$INPUT_FILE"
    printf "\n"
} >> "$OUTPUT_FILE"
