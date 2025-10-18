# GitHub Repository Setup Guide

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

---

## Table of Contents

1. [Initial Repository Setup](#1-initial-repository-setup)
2. [Enable GitHub Features](#2-enable-github-features)
3. [Pull Request Settings](#3-pull-request-settings)
4. [Configure Branch Protection](#4-configure-branch-protection)
5. [Protect Develop Branch](#5-protect-develop-branch-optional)
6. [Enable Security Features](#6-enable-security-features)
7. [Configure Secrets](#7-configure-secrets)
8. [Enable Actions and Workflows](#8-enable-actions-and-workflows)
9. [Verify and Run Workflows](#9-verify-and-run-workflows)
10. [Set Up Labels](#10-set-up-labels)
11. [Configure Discussions](#11-configure-discussions)
12. [Set Repository Topics](#12-set-repository-topics)
13. [Create First Release](#13-create-first-release)
14. [Enable GitHub Pages](#14-enable-github-pages-optional)
15. [Configure Notifications](#15-configure-notifications)
16. [Optional Enhancements](#16-optional-enhancements)
17. [Final Verification](#17-final-verification)
18. [Troubleshooting](#troubleshooting)

---

## 1. Initial Repository Setup

### Prerequisites
- You must have admin access to the repository
- The repository should already exist on GitHub (github.com/joligej/jmaths)

### Navigate to Repository
1. Go to https://github.com/joligej/jmaths
2. Click on **Settings** tab (near the top right)

### Update General Settings

1. In **Settings** → **General**
2. Update **Description**:
   ```
   A professional, high-performance C++23 library for arbitrary-precision arithmetic with unlimited-size integers, rational numbers, and advanced mathematical operations
   ```
3. Update **Website**: Add your documentation URL (if you set up GitHub Pages - see section 14)

---

## 2. Enable GitHub Features

### Step 2: General Features

1. In Settings, go to **General** section (left sidebar)
2. Scroll to **Features** section
3. Enable the following checkboxes:
   - ☑ **Issues** - For bug reports and feature requests
   - ☑ **Discussions** - For community Q&A
   - ☑ **Projects** - For roadmap and planning (optional)
   - ☐ **Wikis** - (Optional, we have comprehensive docs)
   - ☐ **Sponsorships** - (Optional for future)

---

## 3. Pull Request Settings

### Step 3: Pull Request Settings

1. In same **General** section, scroll to **Pull Requests**
2. Enable:
   - ☑ **Allow squash merging** - Keeps history clean
   - ☑ **Allow merge commits** - For preserving PR structure
   - ☑ **Allow rebase merging** - For linear history
   - ☑ **Automatically delete head branches** - Keeps repo tidy
   - ☑ **Allow auto-merge** - Enables auto-merge when checks pass

---

## 4. Configure Branch Protection

**⚠️ CRITICAL:** Complete this AFTER Step 9 (running workflows at least once). Status checks must exist before they can be made required.

### Step 4: Protect Main Branch (WAIT until after Step 9!)

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
- In the **Search for status checks** field, type and select the following checks (they will appear after Step 9):
  - Type `build-and-test` and select:
    - `build-and-test (ubuntu-latest, Release)`
    - `build-and-test (ubuntu-latest, Debug)`
  - Type `code-quality` and select it
  - Type `static-analysis` and select it
  - Type `coverage` and select it (if using code coverage)

**Additional protections:**
- ☑ **Require conversation resolution before merging**
- ☑ **Require signed commits** (highly recommended for security)
- ☑ **Require linear history** (recommended for clean git history)
- ☑ **Include administrators** (ensures even admins follow the rules)

5. Click **Create** or **Save changes**

---

## 5. Protect Develop Branch (Optional)

### Step 5: Protect Develop Branch (WAIT until after Step 9!)

If you use a `develop` branch:

1. Repeat Step 4 with branch name pattern: `develop`
2. Consider slightly relaxed rules for develop (e.g., no required approvals)

---

## 6. Enable Security Features

### Step 6: Enable Security Features

1. Go to **Settings** → **Code security and analysis**
2. Enable the following features:

**Dependency graph:**
- ☑ **Automatically enable for new private repositories**

**Dependabot:**
- ☑ **Dependabot alerts** - Get notified of vulnerable dependencies
- ☑ **Dependabot security updates** - Automatically create PRs to fix vulnerabilities
- ☑ **Dependabot version updates** - Keep dependencies up-to-date (uses .github/dependabot.yml)

**Code scanning:**
- ☑ **CodeQL analysis** - Advanced security analysis
  - Click **Set up** → **Default** (uses your .github/workflows/security.yml)

**Secret scanning:**
- ☑ **Secret scanning** - Detect accidentally committed secrets
- ☑ **Push protection** - Block pushes that contain secrets

---

## 7. Configure Secrets

### Step 7: Add Repository Secrets

If your workflows require secrets (e.g., for Codecov):

1. Go to **Settings** → **Secrets and variables** → **Actions**
2. Click **New repository secret**
3. Add required secrets:
   - **CODECOV_TOKEN** (if using Codecov for coverage reports)
   - Any other API tokens your workflows need

---

## 8. Enable Actions and Workflows

### Step 8: Enable GitHub Actions

1. Go to **Settings** → **Actions** → **General**
2. Under **Actions permissions**:
   - ☑ **Allow all actions and reusable workflows**
3. Under **Workflow permissions**:
   - ☑ **Read and write permissions**
   - ☑ **Allow GitHub Actions to create and approve pull requests**
4. Click **Save**

---

## 9. Verify and Run Workflows

### Step 9: Verify and Run Workflows

**⭐ CRITICAL STEP - Do this before setting up branch protection!**

1. Go to **Actions** tab in your repository
2. You should see your workflows listed:
   - CI
   - Code Coverage
   - Security
   - Dependency Check
   - Documentation
   - Release (only runs on tags)

3. **Manually trigger each workflow:**
   - Click on each workflow name
   - Click **Run workflow** button (on the right)
   - Select branch: `main`
   - Click green **Run workflow** button

4. **Wait for workflows to complete:**
   - Watch each workflow run to completion
   - All should show green checkmarks ✅
   - If any fail, debug and fix before proceeding

5. **Verify status checks exist:**
   - Once workflows run successfully, their names become available as status checks
   - Now you can go back to Step 4 and set up branch protection

**Troubleshooting:**
- If workflows don't run: Check Settings → Actions → General → Actions permissions
- If workflows fail: Click on the failed run to see error details
- Common issues: Missing secrets, permissions problems, configuration errors

---

## 10. Set Up Labels

### Step 10: Create Labels

GitHub provides default labels, but you can add custom ones:

1. Go to **Issues** tab
2. Click **Labels**
3. Consider adding these labels:
   - `priority: high` (color: #d73a4a)
   - `priority: medium` (color: #fbca04)
   - `priority: low` (color: #0e8a16)
   - `type: performance` (color: #1d76db)
   - `type: algorithm` (color: #5319e7)
   - `good first issue` (color: #7057ff) - For new contributors
   - `help wanted` (color: #008672) - Seeking contributors

---

## 11. Configure Discussions

### Step 11: Set Up Discussion Categories

1. Go to **Discussions** tab (if enabled in Step 2)
2. Click **Edit** (pencil icon) to manage categories
3. Create categories:
   - **Q&A** - Questions and answers
   - **Ideas** - Feature requests and suggestions
   - **Show and Tell** - Share your projects using jmaths
   - **Announcements** - Official project updates

### Step 12: Create Welcome Discussion

Create a pinned welcome discussion:

**Title:** Welcome to jmaths Discussions!

**Content:**
```markdown
# Welcome to the jmaths Community! 👋

We're excited to have you here. This is a place to:

## Getting Started
- Ask questions about using the library
- Share your projects built with jmaths
- Discuss ideas for new features
- Get help from the community

## Guidelines
- Be respectful and welcoming to all community members
- Search existing discussions before creating new ones
- Follow our [Code of Conduct](../CODE_OF_CONDUCT.md)
- Check the [documentation](doc/DOCUMENTATION_INDEX.md) for common questions

## How to Use Discussions

**Q&A** - Ask questions about installation, usage, or troubleshooting
**Ideas** - Propose new features or improvements
**Show and Tell** - Share your projects or interesting use cases
**Announcements** - Stay updated with official project news

## Contributing

Interested in contributing? Check out our [Contributing Guide](doc/CONTRIBUTING.md)!

---

Happy coding! 🚀
```

---

## 12. Set Repository Topics

### Step 13: Add Topics

Topics help others discover your repository.

1. Go to main repository page
2. Click the gear icon ⚙️ next to "About" (top right)
3. Add these topics:
   - `cpp`
   - `cpp23`
   - `arbitrary-precision`
   - `bigint`
   - `bignum`
   - `rational-numbers`
   - `mathematics`
   - `number-theory`
   - `cryptography`
   - `high-performance`
   - `header-only`
   - `boost`

4. Check the boxes:
   - ☑ **Releases** - Show releases
   - ☑ **Packages** - Show packages (if applicable)

---

## 13. Create First Release

### Step 13: Creating Your First Release (v2.0.0)

1. Go to **Releases** (right sidebar on main page)
2. Click **Create a new release** or **Draft a new release**
3. Click **Choose a tag**
4. Type: `v2.0.0`
5. Click **+ Create new tag: v2.0.0 on publish**
6. Set **Release title**: `v2.0.0 - Initial Public Release`
7. Add release notes (copy from CHANGELOG.md)
8. Check ☑ **Set as the latest release**
9. Click **Publish release**

This will automatically trigger your Release workflow to build artifacts.

### Future Releases

For future releases:
1. Update version in relevant files
2. Update CHANGELOG.md with changes
3. Commit and push changes
4. Create and push a new tag: `git tag v2.x.x && git push origin v2.x.x`
5. Or use GitHub's release interface as above

---

## 14. Enable GitHub Pages (Optional)

### Step 14: GitHub Pages for Documentation

**Option A: Simple Documentation (Recommended for Start)**

1. Go to **Settings** → **Pages**
2. Under **Source**, select **Deploy from a branch**
3. Select branch: `main` and folder: `/doc` or `/docs`
4. Click **Save**
5. Your docs will be available at: `https://joligej.github.io/jmaths/`

**Option B: Professional Documentation with MkDocs or Doxygen**

If you want to generate professional API documentation:

1. Set up MkDocs or Doxygen in your repository
2. Use GitHub Actions to build and deploy documentation
3. Configure Pages to deploy from `gh-pages` branch

---

## 15. Configure Notifications

### Step 15: Notification Settings

Customize notifications to stay informed:

1. Click your profile icon → **Settings**
2. Go to **Notifications** (left sidebar)
3. Under **Watching**:
   - Choose notification preferences for repositories you watch
4. Under **Subscriptions**:
   - Configure which events trigger notifications

For this repository specifically:
1. Go to the jmaths repository
2. Click **Watch** (top right)
3. Select **Custom**
4. Check:
   - ☑ Issues
   - ☑ Pull requests
   - ☑ Releases
   - ☑ Discussions
   - ☑ Security alerts

---

## 16. Optional Enhancements

### Code Quality Integrations (Optional but Recommended)

**Codecov (Code Coverage Reports):**
1. Visit https://codecov.io
2. Sign in with GitHub
3. Add the jmaths repository
4. Copy the CODECOV_TOKEN
5. Add it as a repository secret (see Step 7)
6. Your coverage workflow will now upload reports

**Codacy (Code Quality Analysis):**
1. Visit https://www.codacy.com
2. Sign in with GitHub
3. Add the jmaths repository
4. Configure analysis settings

**SonarCloud (Advanced Code Analysis):**
1. Visit https://sonarcloud.io
2. Sign in with GitHub
3. Add the jmaths repository
4. Configure quality gates

### Additional Features

**Projects:**
- Use GitHub Projects for planning and tracking work
- Create a project board with columns: To Do, In Progress, Done
- Link issues and PRs to project cards

**Sponsors:**
- Set up GitHub Sponsors if you want to accept donations
- Configure sponsor tiers and benefits

**Repository Insights:**
- Monitor traffic, popular content, and community engagement
- Use insights to improve documentation and features

**Add Badges to README:**
Add status badges to show build status, coverage, etc.:
```markdown
![CI](https://github.com/joligej/jmaths/workflows/CI/badge.svg)
![Coverage](https://codecov.io/gh/joligej/jmaths/branch/main/graph/badge.svg)
![License](https://img.shields.io/badge/license-GPL--3.0-blue.svg)
```

**Social Preview Image:**
1. Go to **Settings** → **Options**
2. Scroll to **Social preview**
3. Upload a custom image (1280x640px recommended)

**Set Up Milestones:**
1. Go to **Issues** → **Milestones**
2. Create milestones for upcoming releases (e.g., v2.1.0, v2.2.0)
3. Assign issues to milestones for planning

---

## 17. Final Verification

### Step 16: Checklist

Verify everything is configured correctly:

**Repository Settings:**
- ✅ Description and website updated
- ✅ Issues, Discussions, Projects enabled
- ✅ Pull request settings configured

**Branch Protection:**
- ✅ Main branch protected with required reviews
- ✅ Required status checks enabled
- ✅ Conversation resolution required
- ✅ Linear history enforced (optional)

**Security:**
- ✅ Dependabot alerts enabled
- ✅ Dependabot security/version updates enabled
- ✅ CodeQL scanning enabled
- ✅ Secret scanning enabled with push protection

**Actions & Secrets:**
- ✅ GitHub Actions enabled with correct permissions
- ✅ All workflows run successfully
- ✅ Required secrets configured (e.g., CODECOV_TOKEN)

**Community:**
- ✅ Labels organized
- ✅ Discussions set up with welcome message
- ✅ Topics added for discoverability

**Documentation:**
- ✅ GitHub Pages enabled (optional)
- ✅ README badges added (optional)

### Step 17: Create a Test PR

Verify branch protection is working:

1. Create a new branch: `git checkout -b test-branch-protection`
2. Make a trivial change (e.g., add a comment)
3. Commit and push: `git push origin test-branch-protection`
4. Create a PR on GitHub
5. Verify:
   - Required status checks run
   - Review is required
   - Cannot merge until checks pass
6. Close the PR without merging
7. Delete the branch

---

## Troubleshooting

### Workflows Not Running

**Problem:** Workflows don't trigger automatically
**Solution:**
1. Check **Settings** → **Actions** → **General**
2. Ensure "Allow all actions" is selected
3. Check workflow permissions are set to "Read and write"
4. Verify workflow YAML syntax is correct

### Status Checks Not Required

**Problem:** Can't find status checks when setting up branch protection
**Cause:** Status checks only appear after workflows run at least once
**Solution:**
1. Remove branch protection temporarily
2. Push a commit to trigger workflows
3. Wait for workflows to complete
4. Re-add branch protection with required checks

### Dependabot PRs Failing with Red X's

**Problem:** Dependabot creates PRs but they immediately fail
**Cause:** Branch protection set up before workflows ran
**Solution 1 (Temporary):**
1. **Settings** → **Branches** → Edit `main` protection rule
2. Temporarily uncheck "Require status checks to pass"
3. Let Dependabot PRs pass (they will still need review)
4. After workflows run successfully, re-enable required checks

**Solution 2 (Permanent Fix):**
1. Close all Dependabot PRs
2. Remove branch protection completely
3. Manually push a commit to trigger workflows
4. Wait for all workflows to complete successfully
5. Re-add branch protection with required checks
6. Dependabot will create new PRs that should pass

### Code Coverage Not Uploading

**Problem:** Coverage workflow runs but doesn't upload to Codecov
**Solution:**
1. Verify CODECOV_TOKEN is set in repository secrets
2. Check Codecov project is properly configured
3. Review coverage workflow logs for errors
4. Ensure coverage files are being generated

---

## Maintenance Tasks

### Regular (Weekly)
- Review and merge Dependabot PRs
- Respond to new issues and discussions
- Review incoming pull requests

### Periodic (Monthly)
- Review and update documentation
- Check security advisories
- Update dependencies manually if needed
- Review repository insights and metrics

### Release Process
1. Update CHANGELOG.md with all changes
2. Update version numbers in code
3. Run full test suite locally
4. Create and push release tag
5. Verify release workflow completes
6. Download and test release artifacts
7. Announce release in Discussions

---

## Summary Checklist

### Essential (Do These First)
- ✅ Enable Issues and Discussions (Step 2)
- ✅ Configure Pull Request settings (Step 3)
- ✅ Enable all security features (Step 6)
- ✅ Enable GitHub Actions (Step 8)
- ✅ Run workflows to verify they work (Step 9)
- ✅ Set up branch protection (Steps 4-5) - AFTER Step 9
- ✅ Add repository topics (Step 12)

### Recommended (Do These Soon)
- ✅ Configure labels (Step 10)
- ✅ Set up Discussions with welcome message (Step 11)
- ✅ Create first release (Step 13)
- ✅ Create test PR to verify protection (Step 17)

### Optional (Nice to Have)
- ⭐ Enable GitHub Pages (Step 14)
- ⭐ Configure notification preferences (Step 15)
- ⭐ Set up third-party integrations (Step 16)
- ⭐ Add README badges (Step 16)
- ⭐ Create project boards (Step 16)
- ⭐ Configure milestones (Step 16)

---

## Additional Resources

- [GitHub Docs: Managing Repositories](https://docs.github.com/en/repositories)
- [GitHub Docs: Security](https://docs.github.com/en/code-security)
- [GitHub Actions Documentation](https://docs.github.com/en/actions)
- [Semantic Versioning](https://semver.org/)
- [Keep a Changelog](https://keepachangelog.com/)

---

**Last Updated:** January 2025
**Maintainer:** Jasper de Smaele
