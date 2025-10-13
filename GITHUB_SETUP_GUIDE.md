# Complete GitHub Integration Setup Guide for jmaths

This guide provides step-by-step instructions for configuring your jmaths repository on GitHub for maximum professionalism and functionality.

## Table of Contents
1. [Initial Repository Setup](#initial-repository-setup)
2. [Enable GitHub Features](#enable-github-features)
3. [Configure Branch Protection](#configure-branch-protection)
4. [Set Up Security Features](#set-up-security-features)
5. [Configure Secrets](#configure-secrets)
6. [Enable Actions and Workflows](#enable-actions-and-workflows)
7. [Set Up Labels](#set-up-labels)
8. [Configure Discussions](#configure-discussions)
9. [Set Repository Topics](#set-repository-topics)
10. [Enable GitHub Pages (Optional)](#enable-github-pages-optional)
11. [Configure Notifications](#configure-notifications)
12. [Final Verification](#final-verification)

---

## Initial Repository Setup

### Prerequisites
- You must have admin access to the repository
- The repository should already exist on GitHub (github.com/joligej/jmaths)

### Step 1: Navigate to Repository
1. Go to https://github.com/joligej/jmaths
2. Click on **Settings** tab (near the top right)

---

## Enable GitHub Features

### Step 2: General Features
1. In Settings, go to **General** section (left sidebar)
2. Scroll to **Features** section
3. Enable the following checkboxes:
   - ☑ **Issues** - For bug reports and feature requests
   - ☑ **Discussions** - For community Q&A
   - ☑ **Projects** - For roadmap and planning
   - ☐ **Wikis** - (Optional, we have comprehensive docs)
   - ☐ **Sponsorships** - (Optional for future)

### Step 3: Pull Request Settings
1. In same **General** section, scroll to **Pull Requests**
2. Enable:
   - ☑ **Allow squash merging** - Keeps history clean
   - ☑ **Allow rebase merging** - For linear history
   - ☑ **Automatically delete head branches** - Keeps repo tidy

---

## Configure Branch Protection

### Step 4: Protect Main Branch
1. Go to **Settings** → **Branches** (left sidebar)
2. Click **Add branch protection rule** or **Add rule**
3. Configure as follows:

**Branch name pattern:** `main`

**Protect matching branches:**
- ☑ **Require a pull request before merging**
  - ☑ Required approvals: **1**
  - ☑ **Dismiss stale pull request approvals when new commits are pushed**
  - ☑ **Require review from Code Owners**
  
- ☑ **Require status checks to pass before merging**
  - ☑ **Require branches to be up to date before merging**
  - Add required checks (these will appear after first workflow run):
    - `build-and-test (ubuntu-latest, Release)`
    - `build-and-test (ubuntu-latest, Debug)`
    - `code-quality`
    - `static-analysis`
  
- ☑ **Require conversation resolution before merging**
- ☑ **Require linear history**
- ☑ **Do not allow bypassing the above settings**

4. Click **Create** or **Save changes**

### Step 5: Protect Develop Branch (if used)
1. Repeat Step 4 with pattern: `develop`
2. Use similar settings but allow maintainers to push directly

---

## Set Up Security Features

### Step 6: Enable Security Features
1. Go to **Settings** → **Code security and analysis** (left sidebar)
2. Enable all security features:

**Dependency graph:**
- Click **Enable** (should be enabled by default)

**Dependabot:**
- ☑ **Dependabot alerts** - Click **Enable**
- ☑ **Dependabot security updates** - Click **Enable**

**Code scanning:**
- Click **Set up** for CodeQL analysis
- Choose **Default** setup
- Click **Enable CodeQL**

**Secret scanning:**
- ☑ **Secret scanning** - Click **Enable**
- ☑ **Push protection** - Click **Enable**

3. Scroll to **Private vulnerability reporting**
- Click **Enable** to allow security researchers to privately report vulnerabilities

---

## Configure Secrets

### Step 7: Add Repository Secrets
1. Go to **Settings** → **Secrets and variables** → **Actions** (left sidebar)
2. Click **New repository secret**

**Add CODECOV_TOKEN (for code coverage):**
1. Go to https://codecov.io
2. Sign in with your GitHub account
3. Add the jmaths repository
4. Copy the upload token
5. Back in GitHub, create new secret:
   - Name: `CODECOV_TOKEN`
   - Value: [paste the token from Codecov]
6. Click **Add secret**

---

## Enable Actions and Workflows

### Step 8: Enable GitHub Actions
1. Go to **Settings** → **Actions** → **General** (left sidebar)
2. Under **Actions permissions**, select:
   - ● **Allow all actions and reusable workflows**
3. Under **Workflow permissions**, select:
   - ● **Read and write permissions**
   - ☑ **Allow GitHub Actions to create and approve pull requests**
4. Click **Save**

### Step 9: Verify Workflows
1. Go to **Actions** tab (main repository page)
2. You should see your workflows listed:
   - CI
   - Code Coverage
   - Dependency Check
   - Documentation
   - Release
   - Security Scan
3. Click **Run workflow** on CI workflow to trigger first run

---

## Set Up Labels

### Step 10: Create Labels
1. Go to **Issues** tab → **Labels**
2. GitHub provides default labels, but add these additional ones:
3. Click **New label** for each:

| Name | Description | Color |
|------|-------------|-------|
| `enhancement` | New feature or request | `#a2eeef` (light blue) |
| `bug` | Something isn't working | `#d73a4a` (red) |
| `documentation` | Documentation improvements | `#0075ca` (blue) |
| `good first issue` | Good for newcomers | `#7057ff` (purple) |
| `help wanted` | Extra attention is needed | `#008672` (green) |
| `question` | Further information requested | `#d876e3` (pink) |
| `wontfix` | This will not be worked on | `#ffffff` (white) |
| `duplicate` | Issue already exists | `#cfd3d7` (gray) |
| `dependencies` | Dependency updates | `#0366d6` (dark blue) |
| `security` | Security related | `#ee0701` (bright red) |
| `performance` | Performance improvements | `#fef2c0` (yellow) |
| `testing` | Test related changes | `#bfdadc` (teal) |

---

## Configure Discussions

### Step 11: Set Up Discussion Categories
1. Go to **Discussions** tab
2. Click **Edit** next to Categories (gear icon)
3. Create these categories:

**Announcements** (Announcement type)
- Description: Official project announcements
- Enable: ☑ Only maintainers can post

**General** (Open discussion)
- Description: General discussions about jmaths

**Q&A** (Q&A type)
- Description: Questions and help requests

**Ideas** (Open discussion)
- Description: Feature requests and improvement ideas

**Show and Tell** (Open discussion)
- Description: Share projects using jmaths

4. Click **Save**

### Step 12: Create Welcome Discussion
1. Click **New discussion**
2. Category: **Announcements**
3. Title: `Welcome to jmaths! 🎉`
4. Body:
```markdown
# Welcome to jmaths Discussions! 🎉

Thank you for your interest in jmaths - a professional, high-performance C++ library for arbitrary-precision arithmetic.

## Getting Started
- 📖 Read the [README](../blob/main/README.md)
- 📚 Check the [API Reference](../blob/main/doc/API_Reference.md)
- 📝 View [Algorithm Documentation](../blob/main/doc/algorithm_documentation.md)
- 🐛 Report bugs in [Issues](../issues)
- 💡 Share ideas here in [Discussions](../discussions)

## How to Use Discussions
- **Q&A**: Ask questions about using jmaths
- **Show and Tell**: Share projects built with jmaths  
- **Ideas**: Propose and discuss new features
- **General**: Anything else related to jmaths

## Code of Conduct
Please read and follow our [Code of Conduct](../blob/main/CODE_OF_CONDUCT.md).

## Contributing
Interested in contributing? Check out our [Contributing Guidelines](../blob/main/CONTRIBUTING.md).

Happy coding! 🚀
```
5. Click **Start discussion**

---

## Set Repository Topics

### Step 13: Add Topics
1. Go to main repository page
2. Click the gear icon (⚙️) next to **About** (top right of page)
3. In **Topics** field, add these tags (press comma or space after each):
   - `cpp`
   - `cpp23`
   - `arbitrary-precision`
   - `mathematics`
   - `bignum`
   - `rational-numbers`
   - `cmake`
   - `header-only-library`
   - `boost-test`
   - `github-actions`

4. Optional: Add a website URL and description:
   - Website: `https://github.com/joligej/jmaths`
   - Description: `Professional C++23 library for arbitrary-precision arithmetic with comprehensive testing`

5. Click **Save changes**

---

## Enable GitHub Pages (Optional)

### Step 14: GitHub Pages for Documentation
1. Go to **Settings** → **Pages** (left sidebar)
2. Under **Source**, select:
   - Branch: `main` (or create `gh-pages` branch)
   - Folder: `/docs` or `/ (root)`
3. Click **Save**

**Note:** You'll need to either:
- Move documentation to `/docs` folder, or
- Create a `gh-pages` branch with generated documentation

---

## Configure Notifications

### Step 15: Notification Settings
1. Go to repository main page
2. Click **Watch** button (top right)
3. Select watching preferences:
   - **All Activity** - Get notified of everything (recommended for maintainers)
   - **Custom** - Select specific events
4. Configure email preferences:
   - Go to your GitHub **Settings** (profile menu)
   - Click **Notifications**
   - Set preferences for jmaths repository

---

## Final Verification

### Step 16: Checklist

Go through this checklist to ensure everything is set up:

#### Repository Settings
- [ ] Issues enabled
- [ ] Discussions enabled
- [ ] Projects enabled
- [ ] Squash merging allowed
- [ ] Auto-delete head branches enabled

#### Branch Protection
- [ ] Main branch protected
- [ ] PR required before merge
- [ ] Status checks required
- [ ] Code owner review required

#### Security
- [ ] Dependabot enabled
- [ ] CodeQL enabled
- [ ] Secret scanning enabled
- [ ] Push protection enabled
- [ ] Private vulnerability reporting enabled

#### Actions & Secrets
- [ ] Actions enabled
- [ ] Workflows have run successfully
- [ ] CODECOV_TOKEN added (if using Codecov)

#### Community
- [ ] Labels created
- [ ] Discussions configured
- [ ] Welcome discussion posted
- [ ] Topics added

#### Documentation
- [ ] README.md is complete
- [ ] CONTRIBUTING.md is present
- [ ] CODE_OF_CONDUCT.md is present
- [ ] SECURITY.md is present
- [ ] CHANGELOG.md is up to date

---

## Testing Your Setup

### Step 17: Create a Test PR
1. Create a new branch locally: `git checkout -b test-pr`
2. Make a small change (e.g., add a comment to README)
3. Commit: `git commit -am "test: verify PR workflow"`
4. Push: `git push -u origin test-pr`
5. On GitHub, create a Pull Request
6. Verify:
   - [ ] CI workflows start automatically
   - [ ] Code owners are automatically assigned
   - [ ] Status checks appear
   - [ ] All checks pass
7. Close the PR (don't merge)
8. Branch should auto-delete

---

## Maintenance Tasks

### Regular (Weekly)
- Review and respond to issues
- Review and merge PRs
- Check Dependabot alerts

### Periodic (Monthly)
- Review security advisories
- Update dependencies
- Review and update documentation

### Release Process
1. Update CHANGELOG.md
2. Create release tag: `git tag v2.0.0`
3. Push tag: `git push origin v2.0.0`
4. Release workflow will create GitHub release automatically
5. Edit release notes if needed

---

## Troubleshooting

### Workflows Not Running
- Check Actions are enabled in Settings → Actions
- Verify workflow files are in `.github/workflows/`
- Check for YAML syntax errors

### Status Checks Not Required
- Status checks only appear after first run
- Re-configure branch protection after first CI run

### Code Coverage Not Uploading
- Verify CODECOV_TOKEN is set correctly
- Check Codecov website for repository status
- Review coverage workflow logs

---

## Additional Resources

- [GitHub Docs - About Protected Branches](https://docs.github.com/en/repositories/configuring-branches-and-merges-in-your-repository/managing-protected-branches)
- [GitHub Docs - Configuring Code Scanning](https://docs.github.com/en/code-security/code-scanning)
- [GitHub Docs - Managing GitHub Actions](https://docs.github.com/en/actions)
- [Codecov Documentation](https://docs.codecov.com/)

---

## Getting Help

If you encounter issues:
1. Check GitHub Status: https://www.githubstatus.com/
2. GitHub Community Forum: https://github.community/
3. Create an issue in this repository
4. Contact GitHub Support (Settings → Support)

---

**Last Updated:** October 2025  
**Maintainer:** Jasper de Smaele (@joligej)
