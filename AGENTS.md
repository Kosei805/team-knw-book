# Workspace Instructions

This workspace contains local, project-scoped skills for Codex under `.codex/skills/`.

## Humanizer JA

When the user asks to humanize Japanese text, remove AI-like phrasing from Japanese writing, or rewrite Japanese prose to sound more natural and human, read and follow `.codex/skills/humanizer-ja/SKILL.md`.

Prefer using that skill over ad hoc rewriting when the request matches.

## Writing Workspace

This repository is a writing workspace.

When you finish writing or editing the book content itself, run `bash build-in-docker.sh` to generate the PDF.
Book content means the manuscript and book configuration files under `articles/` that affect the rendered book, such as `articles/*.re`, `articles/catalog.yml`, and `articles/config.yml`.
Do not run the build for edits that only touch workspace docs, agent instructions, or other non-book notes.

When touching writing-related files, use `.codex/skills/humanizer-ja/SKILL.md` as part of the editing workflow so Japanese prose stays natural and does not read like AI-generated text.

When writing technical book content, also use `.codex/skills/content-first-writing/SKILL.md` so the chapter is shaped by user input before any prose is drafted.

Writing-related files in this workspace:

- `articles/catalog.yml`: Table of contents order for the book. Use this to determine which `.re` file corresponds to each section, and add new chapter filenames here when creating additional chapters.
- `articles/config.yml`: Book metadata such as title and author.
- `articles/*.re`: Manuscript files. Each chapter maps to one `.re` file. When creating a new chapter file, also add its filename to `articles/catalog.yml`.
- `articles/doc/format.ja.md`: Re:VIEW format guide for this repository. Read this before editing manuscript structure or syntax in `.re` files, and follow it when writing headings, paragraphs, lists, columns, and other markup.
