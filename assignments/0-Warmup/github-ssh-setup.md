# Setting Up SSH Access to GitHub

This guide will help you set up SSH key authentication with GitHub, which allows you to push and pull code without entering your password each time.

## Why Use SSH?

- **No password prompts**: Once configured, you won't need to authenticate for every push/pull
- **More secure**: SSH keys are more secure than password authentication
- **Industry standard**: Learning SSH is valuable for professional development work

## Prerequisites

- A GitHub account
- Access to a Linux terminal (tux cluster or your local Linux VM)
- Git installed (`git --version` should work)

---

## Step 1: Check for Existing SSH Keys

First, check if you already have SSH keys:

```bash
ls -la ~/.ssh
```

Look for files named:
- `id_rsa` and `id_rsa.pub` (older RSA keys)
- `id_ed25519` and `id_ed25519.pub` (newer Ed25519 keys, recommended)

If you see these files, **you already have SSH keys** and can skip to Step 3. Otherwise, continue to Step 2.

---

## Step 2: Generate a New SSH Key

We'll generate an Ed25519 key (recommended by GitHub):

```bash
ssh-keygen -t ed25519 -C "yourname@drexel.edu"
```

**If Ed25519 is not supported** (rare, but possible on older systems):
```bash
ssh-keygen -t rsa -b 4096 -C "yourname@drexel.edu"
```

### During Key Generation:

1. **File location prompt**:
   ```
   Enter file in which to save the key (/home/username/.ssh/id_ed25519):
   ```
   Press **Enter** to accept the default location.

2. **Passphrase prompt**:
   ```
   Enter passphrase (empty for no passphrase):
   ```
   
   You have two options:
   - **Press Enter twice** for no passphrase (simpler, but less secure)
   - **Enter a passphrase** (more secure, but you'll need to enter it occasionally)
   
   For this class, no passphrase is fine, but use a passphrase for professional work.

You should see output like:
```
Your identification has been saved in /home/username/.ssh/id_ed25519
Your public key has been saved in /home/username/.ssh/id_ed25519.pub
The key fingerprint is:
SHA256:xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx yourname@drexel.edu
```

---

## Step 3: Copy Your Public Key

Display your public key:

```bash
cat ~/.ssh/id_ed25519.pub
```

Or if you used RSA:
```bash
cat ~/.ssh/id_rsa.pub
```

**Copy the entire output**. It should look like:
```
ssh-ed25519 AAAAC3NzaC1lZDI1NTE5AAAAIxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx yourname@drexel.edu
```

**Important**: Copy the **entire line**, from `ssh-ed25519` (or `ssh-rsa`) through your email address.

---

## Step 4: Add SSH Key to Your GitHub Account

1. **Go to GitHub** and log in

2. **Click your profile picture** (top right) → **Settings**

3. **In the left sidebar**, click **SSH and GPG keys**

4. **Click** the green **"New SSH key"** button

5. **Fill in the form**:
   - **Title**: Give it a descriptive name (e.g., "Drexel tux cluster" or "My Linux VM")
   - **Key type**: Leave as "Authentication Key"
   - **Key**: Paste your public key (the entire line you copied in Step 3)

6. **Click** "Add SSH key"

7. You may be prompted to confirm with your GitHub password

---

## Step 5: Test Your SSH Connection

Test that GitHub recognizes your key:

```bash
ssh -T git@github.com
```

**First time only**: You'll see a message about authenticity:
```
The authenticity of host 'github.com (140.82.113.4)' can't be established.
ED25519 key fingerprint is SHA256:+DiY3wvvV6TuJJhbpZisF/zLDA0zPMSvHdkr4UvCOqU.
Are you sure you want to continue connecting (yes/no/[fingerprint])?
```

Type **`yes`** and press Enter.

**Success looks like**:
```
Hi yourusername! You've successfully authenticated, but GitHub does not provide shell access.
```

This is normal! It confirms SSH is working.

**If you see an error** like "Permission denied", double-check:
- You copied the correct public key (not the private key)
- You pasted the entire key into GitHub
- You're using the correct key file name in your commands

---

## Step 6: Update Your Git Remote (If Already Cloned via HTTPS)

If you already cloned your repository using HTTPS, you need to update it to use SSH:

```bash
# Navigate to your repository
cd ~/cs283-wi26-yourusername

# Check current remote URL
git remote -v

# If it shows https://, update to SSH:
git remote set-url origin git@github.com:drexel-cs-283/cs283-wi26-yourusername.git
```

Replace `cs283-wi26-yourusername` with your actual repository name.

**Verify the change**:
```bash
git remote -v
```

Should now show:
```
origin  git@github.com:drexel-cs-283/cs283-wi26-yourusername.git (fetch)
origin  git@github.com:drexel-cs-283/cs283-wi26-yourusername.git (push)
```

---

## Step 7: Test Push Access

Test that you can push:

```bash
# Make a small change
echo "# SSH Setup Complete" >> README.md

# Commit
git add README.md
git commit -m "Test SSH push access"

# Push
git push origin main
```

If this works **without asking for a password**, you're all set! 🎉

---

## Future Repository Clones

From now on, when cloning repositories, use the SSH URL instead of HTTPS:

**SSH URL format**:
```bash
git clone git@github.com:organization/repository.git
```

**Not HTTPS**:
```bash
# Don't use this anymore
git clone https://github.com/organization/repository.git
```

You can find the SSH URL on any GitHub repository by clicking the green "Code" button and selecting the "SSH" tab.

---

## Troubleshooting

### Problem: "Permission denied (publickey)"

**Solutions**:
1. Verify your public key is added to GitHub (Settings → SSH and GPG keys)
2. Make sure you copied the **public** key (`.pub` file), not the private key
3. Test the connection: `ssh -T git@github.com`
4. Check file permissions:
   ```bash
   chmod 700 ~/.ssh
   chmod 600 ~/.ssh/id_ed25519
   chmod 644 ~/.ssh/id_ed25519.pub
   ```

### Problem: "Could not open a connection to your authentication agent"

This usually means the SSH agent isn't running. Start it:
```bash
eval "$(ssh-agent -s)"
ssh-add ~/.ssh/id_ed25519
```

### Problem: Using tux cluster and getting "Too many authentication failures"

The tux cluster may have multiple keys. Specify which key to use:
```bash
ssh -i ~/.ssh/id_ed25519 -T git@github.com
```

Or create/edit `~/.ssh/config`:
```bash
cat >> ~/.ssh/config << EOF
Host github.com
    HostName github.com
    User git
    IdentityFile ~/.ssh/id_ed25519
    IdentitiesOnly yes
EOF
```

---

## Security Best Practices

1. **Never share your private key** (the file without `.pub`)
2. **Never commit your private key** to a repository
3. **Use a passphrase** for keys used in professional/production environments
4. **Use different keys** for different machines (tux, personal laptop, etc.)
5. **Remove old keys** from GitHub when you stop using a machine

---

## Additional Resources

- [GitHub's official SSH documentation](https://docs.github.com/en/authentication/connecting-to-github-with-ssh)
- [Understanding the SSH protocol](https://www.ssh.com/academy/ssh/protocol)

---

**Questions?** Ask during office hours or post on the course discussion board.