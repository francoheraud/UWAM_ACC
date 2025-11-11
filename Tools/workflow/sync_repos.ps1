# sync_repos.ps1 (test 2)
# PowerShell script to commit and push to two remotes


param (
    [string]$Message = "Auto commit"
)

# Ensure script stops on errors
$ErrorActionPreference = "Stop"

# Navigate to repo directory (optional)
Set-Location "C:\Users\Franc\OneDrive\Documents\GitHub\UWAM_ACC"

# Add all changes
git add -A

# Commit with a message
git commit -m "$Message"

# Push to first remote (e.g. GitHub)
Write-Host "Pushing to origin..."

git fetch origin
git push --force origin main

# Push to second remote (e.g. GitLab)
Write-Host "Pushing to gitlab..."

git fetch uwam
git pull --rebase uwam main
git push --force uwam main

git status

