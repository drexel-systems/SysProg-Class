# Extra Credit: GitHub Actions CI/CD with AI-Assisted Learning

**Points:** 5  
**Difficulty:** Intermediate - Requires Independent Research  
**Skills:** CI/CD, YAML, Self-Directed Learning

---

## The Challenge

In professional software development, every time you push code to a repository, automated systems compile your code, run tests, and check for problems. This is called **Continuous Integration (CI)**, and it's how modern software teams catch bugs before they reach production.

Your challenge: Use GitHub Actions to automatically compile and test your warmup assignment every time you push code to your repository.

**Specifically, you need to:**
1. Automatically compile your C program when you push changes
2. Automatically run your pytest test suite
3. See pass/fail results directly in GitHub
4. Learn how to do this using AI tools (ChatGPT, Claude, Gemini, etc.)

**The catch:** I'm not giving you step-by-step instructions. You'll research and figure this out yourself using AI assistance.

---

## Why This Matters

- **Industry skill**: CI/CD is used in virtually every professional software team
- **Free tool**: GitHub Actions is built into GitHub - no setup needed
- **Career preparation**: Learning to learn new technologies independently is more valuable than memorizing specific commands
- **Modern approach**: Using AI to research and understand new tools is becoming standard practice

---

## Getting Started: Key Questions to Explore

You have our entire repository with multiple assignment directories (`0-Warmup/`, `01-Assignment/`, etc.). Here are some questions to guide your research:

### Discovery Phase

Start by asking AI tools these types of questions:

1. **What is GitHub Actions?** How does it relate to CI/CD?

2. **Where do workflow files live?** Is there a special directory structure GitHub expects?

3. **How do you trigger workflows?** Can they run automatically when you push code?

4. **This is important:** You have multiple assignment directories in one repository. How can you create a workflow that ONLY runs when files in `0-Warmup/` change? You don't want it running when you modify other assignments.

5. **How do workflows run commands?** You need to run `make` and `pytest` - how does that work in a workflow?

### Implementation Phase

Once you understand the basics:

6. What does a workflow file look like? (It's written in YAML)

7. How do you install tools like Python and gcc in a workflow?

8. How do you run commands in a specific directory within a workflow?

9. Where do you see the results of workflow runs?

---

## Learning Strategy: Using AI Effectively

This assignment requires you to research GitHub Actions independently using AI tools. Here's how to approach it:

### General Approach

1. **Start broad, get specific**: Begin with "What is GitHub Actions?" then drill down to your specific needs
2. **Provide context**: Tell the AI you're working with a C program, Makefile, pytest tests, and multiple assignment directories
3. **Iterate**: When you get an answer, test it, see what works, ask follow-up questions
4. **Compare**: Try the same question on different AI tools - you'll learn more from comparing their answers
5. **Validate**: When the AI gives you YAML code, try to understand what each line does before using it

### When You Get Stuck

- Share your error messages with the AI
- Show your workflow file and ask what's wrong
- Ask "why" questions to understand, not just "how" questions to copy-paste
- Look at the GitHub Actions logs - they're surprisingly helpful

### Critical Thinking

AI tools are powerful but not perfect:
- They might suggest outdated approaches
- They might overcomplicate things
- They might give you code that works but you don't understand

Your job is to understand, not just to get something working.

---

## What You Need to Deliver

### 1. A Working GitHub Actions Workflow

Create a workflow that:
- Compiles your warmup assignment's C code
- Runs your pytest tests
- Only triggers when you change files in the `0-Warmup/` directory (not other assignments)
- Shows results in GitHub's Actions tab

### 2. Documentation: `0-Warmup/ACTIONS-LEARNING.md`

Write a document that demonstrates you learned something, not just copied code. Include:

**Your Learning Journey**
- What questions did you ask AI tools?
- What did you discover about GitHub Actions?
- What didn't work at first and how did you debug it?
- Show 2-3 example prompts you used (screenshots or copy-paste)

**Technical Explanation**
- How does your workflow work? Explain in your own words.
- Why does it only run for `0-Warmup/` changes?
- What would you need to do to add workflows for other assignments?

**Evidence**
- Screenshots showing: (1) successful workflow run in Actions tab, (2) the detailed logs

---

## How You'll Be Graded

**5 points** - Workflow works correctly, runs only for `0-Warmup/`, comprehensive documentation shows genuine learning and understanding

**4 points** - Workflow works but might run for all changes, good documentation of learning process

**3 points** - Workflow runs with minor issues, basic documentation

**2 points** - Workflow partially works, minimal documentation

**0 points** - Workflow doesn't work or documentation missing

---

## Hints for Success

- **Start simple**: Get ANY workflow running first, even if it just says "hello world"
- **Build incrementally**: Once a workflow runs, add compilation. Then add testing. Then add path filtering.
- **Read the error messages**: GitHub Actions shows detailed logs - they tell you exactly what went wrong
- **Test often**: Make small changes, commit, push, see what happens
- **Document as you go**: Take notes on what you discover - you'll need this for your writeup

---

## Resources

- GitHub Actions Documentation (Google it or ask AI for the link)
- Your AI tool of choice (ChatGPT, Claude, Gemini, etc.)
- The GitHub Actions tab in your repository (this is where you see results)

---

## A Final Thought

The goal isn't just to get a green checkmark. It's to understand how and why it works. When you're done, you should be able to explain GitHub Actions to someone else and create workflows for future assignments on your own.

**Good luck discovering!**