# Seeds site theme

The look of the [Seeds site](https://github.com/AhmedAldeek03/seeds_site) template. A
Seeds UI subtheme: Seeds UI provides the layout and components, this theme adds the
styles and two template fixes.

| File | What it does |
|---|---|
| `css/site.css` | Header, menu, page title and tabs, cards, forms and footer. Change the colours in the `:root` variables at the top. |
| `templates/navigation/menu.html.twig` | A parent menu item with its own page (for example *Media center*) stays a link and opens its dropdown on hover. Seeds UI makes every parent a toggle with no link. |
| `templates/navigation/menu-local-task.html.twig` | Removes the tabs toggle button that Seeds UI shows without its script. |
| `config/install/seeds_site_theme.settings.yml` | The Seeds UI settings, which Seeds UI reads from the active theme. Also fixes a PHP warning on the login page. |

Plain CSS, no build step. The Seeds site recipe installs this theme and makes it the
default; to use it on its own:

```bash
composer require drupal/seeds_site_theme
drush theme:enable seeds_site_theme
drush config:set system.theme default seeds_site_theme
```
