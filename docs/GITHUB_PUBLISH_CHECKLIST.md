# GitHub Publish Checklist (Beginner Friendly)

Use this checklist to publish this project to your GitHub account.

## 0) Make sure you are in project folder

```bash
cd /home/kali/esp32-wroom32-mini
```

## 1) Verify files to publish

Recommended files/folders:

- `README.md`
- `easydoesit.png`
- `nano_rgb_test/`
- `src/`
- `platformio.ini`
- `docs/`
- `.gitignore`

Do **not** commit build cache folders like `.pio/`.

## 2) Initialize git (if not already)

```bash
git init
git add .
git status
```

## 3) First commit

```bash
git commit -m "Initial commit: Arduino Nano 1x8 RGB beginner project"
```

## 4) Create a GitHub repo

- Go to GitHub -> New repository
- Suggested name: `beginner-nano-rgb-guide`
- Add description: `Beginner Arduino Nano + WS2812 1x8 RGB tutorial`
- Keep it empty (no README from GitHub side, because this repo already has one)

## 5) Connect local repo to GitHub

Replace `<YOUR_GITHUB_USERNAME>` with your username:

```bash
git remote add origin https://github.com/<YOUR_GITHUB_USERNAME>/beginner-nano-rgb-guide.git
git branch -M main
git push -u origin main
```

## 6) Optional polish before sharing

- Add a license (MIT is common for beginner tutorials)
- Add tags/releases
- Pin repo on your GitHub profile
- Add one short demo video or GIF in README

## 7) Verify published repo

After push:

- Open the repo page
- Confirm README renders with image
- Confirm all code files are present
- Confirm no large temporary build folders were uploaded
