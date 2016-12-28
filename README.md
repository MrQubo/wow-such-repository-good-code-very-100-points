# Topcoder site - Misc Bug Fixes  submission

Clone dev branch from [https://github.com/appirio-tech/topcoder-app.git](https://github.com/appirio-tech/topcoder-app.git)

Copy files from `topcoder-app/` to clonned `topcoder-app` repo root folder

For installation and testing refer to [https://github.com/appirio-tech/topcoder-app](https://github.com/appirio-tech/topcoder-app)


## Changes

### 1) Dashboard - Challenge card links do not work

	Adding target="_self" in the browser inspector made the page reload.
	Tested on:
		- Linux-Ubuntu 16.04.1
			- Firefox
			- Chromium
		- Windows 10
			- Firefox
			- Chrome
			- Edge

### 2) Fix Responsiveness - Community Overivew page

	Now displays two stats in a column under 768px width and all four under 576px.

	Tested on:
		- Linux-Ubuntu 16.04.1
			- Firefox
			- Chromium

	Added $number-of-stats Sass variable for easier future changes

### 3) Replace "Load More" with infinite scroll for Challenges page

	Tested on:
		- Linux-Ubuntu 16.04.1
			- Firefox
			- Chromium
