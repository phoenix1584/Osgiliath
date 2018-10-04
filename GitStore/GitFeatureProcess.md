# Git Feature Lifecycle for developers 

## Starting with a fresh checkout

* `git clone git@github.com:main/code-repo.git`
  
  [**Optional**]
  * git config user.name _username_
  * git config user.email _user.email@domain.com_
* `git checkout master`
* `git flow init -d` 
  <details><summary><i>Expected Output</i></summary>
  <p>

  ```
  Using default branch names.

  Which branch should be used for bringing forth production releases?
    - develop
    - master
  Branch name for production releases: [master] 

  Which branch should be used for integration of the "next release"?
    - develop
  Branch name for "next release" development: [develop] 

  How to name your supporting branch prefixes?
  Feature branches? [feature/] 
  Release branches? [release/] 
  Hotfix branches? [hotfix/] 
  Support branches? [support/] 
  Version tag prefix? [] 
  ```
  </p>
  </details>

* `git flow feature start feature-name`
  <details><summary><i>Expected Output</i></summary>
  <p>

  ```

  Switched to a new branch 'feature/feature-name'

  Summary of actions:
  - A new branch 'feature/feature-name' was created, based on 'develop'
  - You are now on branch 'feature/feature-name'

  Now, start committing on your feature. When done, use:

      git flow feature finish feature-name

  ```
  </p>
  </details>

* `git flow feature publish feature-name`
  <details><summary><i>Expected Output</i></summary>
  <p>

  ```
  Total 0 (delta 0), reused 0 (delta 0)
  remote:
  remote: Create a pull request for 'feature/feature-name' on GitHub by visiting:
  remote:      https://github.com:main/code-repo/pull/new/feature/feature-name
  remote:
  To github.com:main/code-repo.git
  * [new branch]      feature/feature-name -> feature/feature-name
  Already on 'feature/feature-name'
  Your branch is up-to-date with 'origin/feature/feature-name'.

  Summary of actions:
  - A new remote branch 'feature/feature-name' was created
  - The local branch 'feature/feature-name' was configured to track the remote branch
  - You are now on branch 'feature/feature-name'
  ```
  </p>
  </details>

* `git commit -m "Changes for feature functionality."`
* `git status`
  <details>
  <summary><i>Expected Output</i></summary>
  <p>

  ```
  $ git status
  On branch feature/feature-name
  Your branch is ahead of 'origin/feature/feature-name' by 1 commit.
    (use "git push" to publish your local commits)
  Untracked files:
    (use "git add <file>..." to include in what will be committed)

  nothing added to commit but untracked files present (use "git add" to track)
  ```
  </p>
  </details>

* `git push`
  <details>
  <summary><i>Expected Output</i></summary>
  <p>

  ```
  Counting objects: 8, done.
  Delta compression using up to 4 threads.
  Compressing objects: 100% (5/5), done.
  Writing objects: 100% (8/8), 1.15 KiB | 1.15 MiB/s, done.
  Total 8 (delta 4), reused 0 (delta 0)
  remote: Resolving deltas: 100% (4/4), completed with 4 local objects.
  To github.com:main/code-repo.git
    751dc75..a8f23b3  feature/feature-name -> feature/feature-name
  ```
  </p>
  </details>

* **Pull Request**
  * Create a pull request for the branch `feature-name` from the github UI(browser).
     * Add at least 2 reviewers. Select reviewers based on their familiarity with the functionality and availability during the sprint.
     * Link the Pull request to an issue.
     * Link dependencies if any.
  * Resolve the issues raised by reviewers (either code commits or explanation comments.)
  * Branch will be ready for a merge once at least 2 reviewers approve it. `Merge Pull Request` button will be enabled on the github UI of the PR page.

* **Merging on UI**
  * Use the `Squash and Merge` option from the `Merge Pull Request` drop down.
  * On confirming submission of `Squash and Merge` option, `Delete Branch` option should appear on UI. 
  * Perform `Delete Branch`

* **Manage Local Checkout after merge**
  * `git checkout develop`
  * `git fetch --all`
  * `git pull`

## Working with Existing Checkout

_**To be completed**_

## References

* [gitflow-workflow](https://www.atlassian.com/git/tutorials/comparing-workflows/gitflow-workflow)
* [gitflow cheatsheet](https://gist.github.com/JamesMGreene/cdd0ac49f90c987e45ac)
* [gitflow plugin](https://github.com/nvie/gitflow)
