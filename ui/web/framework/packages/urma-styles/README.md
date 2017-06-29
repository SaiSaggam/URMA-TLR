# Overview

This package contains the common styles and custom fonts for the URMA framework.

## Styles
Our styles are based on the [Bourbon](http://bourbon.io) Sass library. We use meteor packages to pull in 3 Bourbon
library components: Bourbon, Neat, and Bitters. There is a fourth Bourbon component called Refills. There is no good
meteor package currently available for this so we have included them ourselves in the styles/bourbon/refills directory.

## Fonts
The corporate branding currently uses google Google fonts. We've included the fonts we use in the fonts directory.
To reduce the amount of space required on devices, we are only using woff/woff2 font file types. Form more information
on browser version support see [Using @face-font | CSS Tricks](https://css-tricks.com/snippets/css/using-font-face/).
We used the [localfont tool](http://www.localfont.com/) to download these fonts.
