# Seeds site theme

The look of the [Seeds site](https://github.com/AhmedAldeek03/seeds_site) template: a
Seeds UI subtheme for organisation websites, in English and Arabic.

- **Brand settings, no code:** primary and accent colours, font, corner style, header
  button, contact details, footer text and social links, at
  *Appearance > Settings > Seeds site theme*. Colours are checked for contrast: links get
  a darker shade if needed, and buttons pick white or dark text, so text always reaches
  WCAG AA (4.5:1).
- **Arabic and right to left:** a right-to-left build of the stylesheet (rtlcss), mirrored
  icons and layouts, and IBM Plex Sans Arabic or Cairo for Arabic text.
- **Every Seeds feature styled:** News, Events, Team, Gallery (with a photo viewer),
  Publications (with download cards), Partners (logo wall and cards), FAQ (accordion), Slider
  (full width), News and Events card carousels (Swiper) and
  Contact us, plus search, login, "Page not found", breadcrumb, tabs, messages, pager and
  the Klaro cookie banner.
- **Sections for Canvas:** Hero, Section, Feature, Key figure, Image and text, Message,
  Milestone, Testimonial, Call to action and Button, in the *Seeds site* group of the Canvas library.

## Seeds conventions

It follows the Seeds UI subtheme kit and the Seeds field conventions:

- Seeds UI is the base theme. Seeds UI ships markup but almost no CSS; this theme styles its
  templates and components (`card-default`, `teaser`, `slider`, `statistic`, `accordion`...).
- Styles are Sass partials with the kit's names (`_base`, `_elements`, `_fields`, `_general`,
  `_displays`, `_views`, `_blocks`, `_layout`, `_overrides`), built with the kit's pipeline
  (Sass, Autoprefixer, rtlcss) into `dist/css/style.css` and `style.rtl.css`.
- Seeds fields are named by type (`field_body`, `field_media`, `field_term`, `field_date`,
  `field_date_range`, `field_link`, `field_media_1`...), so styles target those names, not
  content types. A new Seeds feature recipe that follows the conventions is styled as is.
- Cards look the same whether they use the plain Card view mode or the Seeds UI card
  component (Display Suite and UI Patterns, as in Media center: Seeds UI cards).

One difference from the kit: `style-ltr` is listed in `seeds_site_theme.info.yml` (the kit
attaches it in `hook_preprocess_html()`), because the Canvas editor preview only loads the
libraries listed there. `hook_css_alter()` swaps it for `style-rtl` on right-to-left pages.

## Theme settings

| Tab | Settings |
|---|---|
| Brand | Primary colour, accent colour, font (one of five self-hosted fonts that cover Arabic and English: IBM Plex Sans + IBM Plex Sans Arabic, Cairo, Tajawal, Plus Jakarta Sans + Tajawal, Almarai; or the system font), corners (sharp, soft, round) |
| Header | Sticky header, top bar, search button and search page, header button text and link |
| Contact details | Phone, email, address: shown in the top bar, the footer and next to the Contact us form |
| Footer | About text, copyright note, registration note, background (tone), contact details on or off, white logo |
| Social media | Facebook, X, Instagram, LinkedIn, YouTube, TikTok, WhatsApp |

Brand settings are written to `public://seeds_site_theme/brand-HASH.css`, which overrides the
CSS custom properties (`--seeds-site-*`) and is added to the theme's global library, so the
Canvas editor uses it too. After changing the settings through a config import, clear the
caches (`drush deploy` does).

## Canvas sections

| Component | What it is |
|---|---|
| Hero | Big heading, text, two buttons and an image. Layouts: text beside the image, image behind the text, centred. As the first component of a page it prints the `<h1>` and the page title is hidden. |
| Section | A full-width band (white, light grey, light brand colour, dark, brand colour) with an optional label, heading and introduction. Its slot takes any components or blocks, stacked or in 2, 3 or 4 columns. |
| Feature | Icon, heading, text and link; card or plain. |
| Key figure | A big number with a label. |
| Image and text | Image beside a heading, formatted text and a button; without an image, a single column of text. |
| Message | A message from the chair, director or founder: portrait, heading, message and signature. |
| Milestone | A year and what happened. Several in a Section make a timeline, down the page or across it. |
| Testimonial | A quote with a name, role and photo. |
| Call to action | A highlighted panel with up to two buttons (brand, dark, accent or light). |
| Button | One button: filled, outline, accent or text with arrow. |

On Canvas pages, sections span the full width; other components and blocks sit in the page
container. Images use Canvas's `canvas:image` component (responsive `srcset`), so the
components need Canvas.

## Templates

| Template | Why |
|---|---|
| `layout/page.html.twig` | Top bar, header with search and button, menu drawer on small screens, page header (breadcrumb, title, tabs), optional sidebar, footer from the theme settings. Prints the main menu once (Seeds UI prints it twice). |
| `navigation/menu.html.twig` | Parents with their own page stay links; a chevron button opens the submenu (hover, keyboard and touch). In the main menu a submenu is a full-width mega panel on large screens: the parent's title, a "View all" link, each link's description (the menu link's *Description* field) and, if set, a promo card (the menu link's image, label and link fields from Seeds template). The footer menu shows nested links as columns with headings. Other menus keep a small dropdown; the mobile drawer shows plain lists. |
| `content/node--full.html.twig` | Full pages: detail fields (`field_date`, `field_date_range`, `field_plain_text`, `field_link`, `field_term`, `field_media_1`) move to a side card. |
| `field/field--field-media-file.html.twig` | Files of the Seeds *file* media type become download cards (name, type, size). |
| `block/block--system-branding-block.html.twig` | Logo keeps its proportions; the link is named after the site. |
| `layout/maintenance-page.html.twig` | A branded maintenance page. |

Canvas pages in listings (search results) show as cards with their title and description,
instead of the whole page.

## Development

The compiled CSS in `dist/` is committed, so sites don't need Node. To change the styles:

```bash
npm install
npm run build   # or: npm run watch
```

| Folder | Contents |
|---|---|
| `scss/` | Sass partials; `scss/components/` styles the Canvas sections |
| `components/` | The Canvas sections (single directory components) |
| `templates/` | Twig overrides; `templates/includes/icon.html.twig` holds the icons |
| `includes/brand.inc` | Brand settings, colour contrast checks and the generated brand stylesheet |
| `fonts/` | IBM Plex Sans, IBM Plex Sans Arabic, Cairo, Tajawal, Almarai and Plus Jakarta Sans (SIL Open Font License 1.1). Each `@font-face` has a `unicode-range`, so a site only downloads the font it uses. |
| `js/main.js` | Sticky header shadow, menu drawer position and the gallery photo viewer |

Menus, the menu drawer and header search use Alpine.js, which Seeds UI loads.

## Known issues

- **Slider (older Slider recipe only):** versions before the switch to Swiper Formatter need the
  Slick library, which they don't install; without it, slides scroll sideways (CSS scroll snap).
  The current Slider recipe uses Swiper Formatter, which ships its own Swiper.

## Credits

Brand icons: [Simple Icons](https://simpleicons.org) (CC0 1.0). Fonts: IBM Plex Sans and IBM
Plex Sans Arabic (IBM), Cairo (The Cairo Project Authors), Tajawal (Boutros Fonts), Almarai
(Boutros Fonts), Plus Jakarta Sans (Tokotype), all SIL Open Font License 1.1.
