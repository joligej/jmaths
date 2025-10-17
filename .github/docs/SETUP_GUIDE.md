# GitHub Integration Setup Guide

**Complete Configuration Guide for jmaths Repository**

This comprehensive guide provides step-by-step instructions for configuring the jmaths repository on GitHub to enable professional development workflows, security features, and community engagement tools.

## ⚠️ IMPORTANT: Setup Order

**Follow steps in this exact order to avoid issues:**

1. Steps 1-3: Enable basic repository features
2. Step 6: Enable security features (Dependabot will create PRs)
3. Step 7: Configure secrets if needed
4. Step 8: Enable Actions
5. **Step 9: Run workflows and verify they pass** ⭐ CRITICAL
6. **THEN go back to Steps 4-5:** Set up branch protection with required checks

**Why this order matters:**
- Branch protection rules (Steps 4-5) require status checks to exist first
- Status checks only appear after workflows run successfully (Step 9)
- If you set up branch protection too early, all PRs (including Dependabot) will fail
- Dependabot PRs created in Step 6 will fail until workflows are working

**Already did Steps 4-5 too early?** See the Troubleshooting section at the end.

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

**IMPORTANT:** Complete Steps 4 and 5 AFTER Step 8 (running workflows at least once). Status checks must exist before they can be made required.

### Step 4: Protect Main Branch (WAIT until after Step 8!)
1. Go to **Settings** → **Branches** (left sidebar)
2. Click **Add branch protection rule** or **Add rule**
3. In the **Branch name pattern** field, type: `main`
4. Configure the following options:

**Require a pull request before merging:**
- ☑ Check this box
- In the sub-options that appear:
  - Set **Require approvals** dropdown to: `1`
  - ☑ Check **Dismiss stale pull request approvals when new commits are pushed**
  - ☑ Check **Require review from Code Owners** (only if you have a CODEOWNERS file)
  
**Require status checks to pass before merging:**
- ☑ Check this box
- ☑ Check **Require branches to be up to date before merging**
- In the **Search for status checks** field, type and select the following checks (they will appear after Step 8):
  - Type `build-and-test` and select:
    - `build-and-test (ubuntu-latest, Release)`
    - `build-and-test (ubuntu-latest, Debug)`
  - Type `code-quality` and select it
  - Type `static-analysis` and select it
  - Type `CodeQL` and select it (from security.yml workflow)
  
**Additional protections:**
- ☑ Check **Require conversation resolution before merging**
- ☑ Check **Require linear history** (prevents merge commits)
- ☑ Check **Do not allow bypassing the above settings** (even admins must follow rules)

5. Scroll to bottom and click **Create** or **Save changes**

**Note:** If you don't see the status checks in the search field, it means the workflows haven't run yet. Complete Step 8 first, wait for workflows to finish, then return to configure these protection rules.

### Step 5: Protect Develop Branch (Optional - WAIT until after Step 8!)
Only follow this step if you use a `develop` branch for ongoing development.

1. In **Settings** → **Branches**, click **Add branch protection rule** again
2. In the **Branch name pattern** field, type: `develop`
3. Configure with these lighter restrictions:
   - ☑ **Require a pull request before merging** (but 0 approvals required for faster iteration)
   - ☑ **Require status checks to pass before merging**
     - Select the same status checks as main branch
   - ☑ **Automatically delete head branches**
4. Click **Create**

**Why lighter restrictions?** The develop branch is for active development, while main is your stable/release branch.

---

## Set Up Security Features

### Step 6: Enable Security Features
1. In **Settings**, find and click **Code security and analysis** in the left sidebar
2. You'll see several security features listed. Enable them as follows:

**Dependency graph:**
- This should already be enabled (shows as "Enabled" with a green checkmark)
- If not enabled, click the **Enable** button
- This allows GitHub to track your project dependencies

**Dependabot alerts:**
- Click **Enable** button
- This notifies you of security vulnerabilities in dependencies
- After enabling, you'll receive alerts when vulnerable dependencies are detected

**Dependabot security updates:**
- Click **Enable** button
- This automatically creates PRs to update vulnerable dependencies
- Dependabot will create pull requests to patch security issues

**Code scanning:**
- Find the **CodeQL analysis** section
- Click **Set up** button
- A modal will appear with two options:
  - Choose **Default** setup (easier)
  - Select languages: `C++` (should be auto-detected)
  - Click **Enable CodeQL**
- This scans your code for security vulnerabilities and coding errors
- Note: You already have a security.yml workflow, so this may show as already configured

**Secret scanning:**
- Click **Enable** button under "Secret scanning"
- This detects committed secrets like API keys and tokens
- After enabling, click **Enable** for **Push protection** too
- Push protection prevents you from accidentally pushing secrets

**Private vulnerability reporting:**
- Scroll down to find this section
- Click **Enable** button
- This allows security researchers to privately report vulnerabilities to you
- They can submit reports through the Security tab

3. After enabling all features, scroll to top and verify all show green "Enabled" status

**Important:** After enabling Dependabot, it may immediately create several PRs for dependency updates. This is normal - review and merge them as appropriate.

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
1. In **Settings**, find and click **Actions** in the left sidebar
2. Then click **General** (sub-menu under Actions)
3. Configure the following:

**Actions permissions:**
- You'll see three radio button options:
  - Select ● **Allow all actions and reusable workflows** (most permissive)
  - This allows your repository to use any GitHub Actions
  
**Workflow permissions:**
- Scroll down to find this section
- You'll see two radio button options:
  - Select ● **Read and write permissions**
  - This allows workflows to modify your repository
- Below that, find and check:
  - ☑ **Allow GitHub Actions to create and approve pull requests**
  - This is needed for Dependabot and automated PRs

4. Scroll to bottom and click **Save** button

### Step 9: Verify and Run Workflows
1. Navigate to the **Actions** tab at the top of the repository page (next to Pull requests)
2. You should see a list of your workflows in the left sidebar:
   - CI
   - Code Coverage  
   - Dependency Check
   - Documentation
   - Release
   - Security Scan

3. **Run the CI workflow manually:**
   - Click on **CI** in the left sidebar
   - Click the **Run workflow** dropdown button (right side)
   - Select branch: `main`
   - Click the green **Run workflow** button
   - Wait for the workflow to complete (may take 5-10 minutes)

4. **Check workflow status:**
   - A new workflow run will appear in the list
   - It will show a yellow circle (⚪) while running
   - Wait for it to complete:
     - Green checkmark (✓) means success
     - Red X (✗) means failure
   - Click on the workflow run to see details
   - Verify all jobs complete successfully:
     - build-and-test (ubuntu-latest, Release)
     - build-and-test (ubuntu-latest, Debug)
     - code-quality
     - static-analysis
     - sanitizers (address, undefined, leak)

5. **Run the Security Scan workflow:**
   - Click **Security Scan** in left sidebar
   - Click **Run workflow** → select `main` → click **Run workflow**
   - Wait for completion (creates the CodeQL status check)

6. **After successful runs:** Now you can return to Steps 4 and 5 to configure branch protection with required status checks!

**Troubleshooting workflow failures:**
- If CI fails, click on the failed job to see error logs
- Common issues: missing dependencies, syntax errors in code
- Fix any issues, commit, and workflows will re-run automatically
- The fixed CI workflow should now work (duplicate line was removed)

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
- Check Actions are enabled in Settings → Actions → General
- Verify workflow files are in `.github/workflows/` directory
- Check for YAML syntax errors: use a YAML validator or look for red X's
- Ensure Actions permissions include "Read and write permissions"

### Status Checks Not Required
- Status checks only appear in branch protection after their first successful run
- Run workflows manually first (Step 9)
- After workflows complete, configure branch protection (Steps 4-5)
- Return to Settings → Branches to add the status checks

### Dependabot PRs Failing with Red X's
**Symptoms:** Dependabot creates PRs with names like "chore(deps): bump actions/..." and they all fail with red X's.

**Root cause:** This happens when:
1. Branch protection requires status checks that haven't run yet (Steps 4-5 done before Step 8)
2. CI workflow has syntax errors (like duplicate lines)
3. Workflows don't have permission to run on Dependabot PRs

**Solutions:**
1. **First, verify CI workflow works:**
   - Go to Actions tab
   - Manually run CI workflow on main branch
   - Fix any errors shown in the logs
   - The duplicate line at line 101 has been removed

2. **If branch protection was set up too early:**
   - Go to Settings → Branches
   - Edit or delete the main branch protection rule
   - Wait for CI to pass on some PRs
   - Then re-enable branch protection with required checks

3. **Close and recreate failed Dependabot PRs:**
   - Go to Pull Requests tab
   - Find the failed Dependabot PRs
   - Close them (click "Close pull request")
   - Go to Settings → Code security and analysis → Dependabot
   - Click "Check for updates" to create fresh PRs
   - New PRs should trigger workflows correctly

4. **Verify Dependabot has permissions:**
   - Settings → Actions → General
   - Ensure "Allow GitHub Actions to create and approve pull requests" is checked
   - This allows Dependabot PRs to trigger workflows

5. **If you want to ignore the failed PRs:**
   - Simply close all failing Dependabot PRs
   - Disable and re-enable Dependabot security updates
   - Fresh PRs will be created that should work correctly

### Code Coverage Not Uploading
- Verify CODECOV_TOKEN is set correctly in Settings → Secrets
- Check Codecov website for repository status
- Review coverage workflow logs for upload errors
- Ensure coverage.yml workflow has correct token reference

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
