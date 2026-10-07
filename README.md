# Responsive Menu for Drupal 7 — Bottom Dropdown Fork

A Drupal 7 fork of the [Responsive Menu module](https://www.drupal.org/project/responsive_menu) that adds a full-width menu opening directly below the site header.

The new mode is designed as a continuation of the header rather than a traditional left/right drawer:

- Opens downward with no page overlay.
- Displays the root menu in responsive columns.
- Orders items from top to bottom within each column.
- Slides into nested submenu panels and back again.
- Opens at the active menu trail on inner pages.
- Keeps linked parent items navigable while using a separate submenu arrow.
- Lets Drupal `<nolink>`/separator labels open their submenu like the arrow.
- Animates the hamburger icon into a close icon.
- Includes keyboard and ARIA support.
- Avoids interfering with Drupal administration pages and includes an Adminimal toolbar compatibility fix.

The original left/right mmenu and optional horizontal Superfish modes remain available.

## Requirements

- Drupal 7
- [Libraries API](https://www.drupal.org/project/libraries)
- jQuery 1.7 or newer when using Superfish
- The mmenu library for left/right drawer modes
- The Superfish library only when its horizontal-menu enhancement is enabled

The bottom-dropdown mode does not require mmenu.

## Installation

1. Copy this directory to `sites/all/modules/responsive_menu`.
2. Enable **Responsive menu** in Drupal.
3. Place the **Responsive menu mobile icon** block in the site header.
4. Visit `/admin/structure/menu/settings`.
5. Select **Bottom dropdown** under **How the mobile menu should open**.
6. Enter the CSS selector for the header element below which the menu should appear.
7. Choose the maximum number of columns and save the configuration.
8. Clear Drupal's caches.

For a theme with different homepage and inner-page header wrappers, enter a comma-separated selector such as:

```text
#site-header, #inner-site-header
```

## Bottom-dropdown behavior

At wide sizes, the configured number of columns is used. The layout automatically reduces to three, two, and one column at narrower widths.

Menu items with children receive a separate arrow control:

- If the item has a URL, its label follows that URL and the arrow opens its submenu.
- If it is a Drupal separator/`<nolink>` item, both its label and arrow open the submenu.

The deepest active-trail submenu is selected when the page loads, making the open menu reflect the current page.

## Optional mmenu installation

The mmenu library is required only for the original left/right drawer modes. Download a compatible release, rename the directory to `mmenu`, and place it at:

```text
sites/all/libraries/mmenu
```

The module expects the distribution files below `sites/all/libraries/mmenu/dist`.

Review the license of the particular mmenu release you install. Later releases use a Creative Commons non-commercial license; earlier compatible releases were available under MIT.

## Optional Superfish installation

Download [Superfish](https://github.com/joeldbirch/superfish), rename its directory to `superfish`, and place it at:

```text
sites/all/libraries/superfish
```

The expected JavaScript path is `sites/all/libraries/superfish/dist/js`.

## Theme compatibility

The toggle block must be inside, or associated with, the configured header element. The dropdown is positioned using that element's width and lower edge.

The supplied CSS is intentionally conservative and can be overridden by a site theme. If you disable **Load the responsive_menu module's CSS**, copy the relevant rules into your theme first.

The compatibility stylesheet corrects Adminimal configurations that use negative toolbar offsets intended for a transformed mmenu page wrapper. Its selectors only affect pages using the corresponding Adminimal classes.

## Advanced customization

The existing `hook_js_alter()` and menu-tree alter hooks from Responsive Menu remain available. See [`responsive_menu.api.php`](responsive_menu.api.php) for module hooks.

## Origin and license

This repository is a modified fork of Drupal Responsive Menu 7.x-3.7. The bottom-dropdown mode and related compatibility work were added in October 2026. See [`NOTICE.md`](NOTICE.md) for details.

The Drupal module is distributed under the GNU General Public License version 2. See [`LICENSE.txt`](LICENSE.txt).
