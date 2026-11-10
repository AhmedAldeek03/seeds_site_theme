<?php

/**
 * @file
 * Seeds site theme settings (Appearance > Settings > Seeds site theme).
 */

declare(strict_types=1);

use Drupal\Core\Form\FormStateInterface;

require_once __DIR__ . '/includes/brand.inc';

/**
 * Implements hook_form_FORM_ID_alter() for system_theme_settings.
 */
function seeds_site_theme_form_system_theme_settings_alter(array &$form, FormStateInterface $form_state, $form_id = NULL): void {
  // Only on this theme's settings page, not on subthemes' or the global one.
  $build_info = $form_state->getBuildInfo();
  if (($build_info['args'][0] ?? NULL) !== 'seeds_site_theme') {
    return;
  }

  $form['seeds_site'] = [
    '#type' => 'vertical_tabs',
    '#title' => t('Seeds site theme'),
    '#weight' => -20,
  ];

  $brand = seeds_site_theme_brand_settings();
  $form['brand'] = [
    '#type' => 'details',
    '#title' => t('Colours'),
    '#group' => 'seeds_site',
    '#tree' => TRUE,
    '#description' => t('Four colours make the whole palette. Light tints, a dark shade and text colours are worked out from them, and every text colour is checked for contrast: if a colour is too light to read, text uses a darker shade of it, and buttons pick white or dark text.'),
    '#attached' => ['library' => ['seeds_site_theme/settings']],
  ];
  $form['brand']['primary'] = [
    '#type' => 'color',
    '#title' => t('Primary colour'),
    '#description' => t('Buttons, links, titles and the Primary background.'),
    '#default_value' => $brand['primary'],
  ];
  $form['brand']['secondary'] = [
    '#type' => 'color',
    '#title' => t('Secondary colour'),
    '#description' => t('Buttons on hover, labels above titles, tags and the Secondary backgrounds.'),
    '#default_value' => $brand['secondary'],
  ];
  $form['brand']['accent'] = [
    '#type' => 'color',
    '#title' => t('Accent colour'),
    '#description' => t('The short line under titles, icons, card borders and other small touches.'),
    '#default_value' => $brand['accent'],
  ];
  $form['brand']['surface'] = [
    '#type' => 'color',
    '#title' => t('Background colour'),
    '#description' => t('Light sections, the page header and cards on white sections. Keep it very light.'),
    '#default_value' => $brand['surface'],
  ];
  $form['brand']['headings'] = [
    '#type' => 'radios',
    '#title' => t('Title colour'),
    '#options' => [
      'primary' => t('Primary colour'),
      'dark' => t('Dark grey'),
    ],
    '#default_value' => $brand['headings'],
  ];
  $palette = seeds_site_theme_palette($brand);
  $form['brand']['preview'] = [
    '#type' => 'inline_template',
    '#template' => '<div class="seeds-site-settings-preview" data-seeds-site-preview style="{{ style }}">
      <p class="seeds-site-settings-preview__label">{{ label }}</p>
      <div class="seeds-site-settings-preview__grid">
        {% for tone, name in tones %}
          <div class="seeds-site-settings-preview__tone seeds-site-settings-preview__tone--{{ tone }}">
            <span class="seeds-site-settings-preview__eyebrow">{{ name }}</span>
            <strong class="seeds-site-settings-preview__title">{{ title }}</strong>
            <span class="seeds-site-settings-preview__rule"></span>
            <span class="seeds-site-settings-preview__text">{{ text }}</span>
            <span class="seeds-site-settings-preview__button">{{ button }}</span>
          </div>
        {% endfor %}
      </div>
    </div>',
    '#context' => [
      'label' => t('Preview of the section backgrounds'),
      'title' => t('Our programmes'),
      'text' => t('Text on this background.'),
      'button' => t('Learn more'),
      'tones' => [
        'white' => t('White'),
        'light' => t('Light'),
        'primary-light' => t('Light primary'),
        'secondary-light' => t('Light secondary'),
        'primary' => t('Primary'),
        'secondary' => t('Secondary'),
        'dark' => t('Dark'),
      ],
      'style' => implode(' ', array_map(static fn ($name, $value) => "--p-$name: $value;", array_keys($palette), $palette)),
    ],
  ];

  $form['typography'] = [
    '#type' => 'details',
    '#title' => t('Font and corners'),
    '#group' => 'seeds_site',
    '#tree' => TRUE,
  ];
  $form['typography']['font'] = [
    '#type' => 'radios',
    '#title' => t('Font'),
    '#options' => [
      'plex' => t('IBM Plex Sans and IBM Plex Sans Arabic (clean and professional)'),
      'cairo' => t('Cairo (rounded; designed for Arabic and English together)'),
      'system' => t("System font (no download; uses each device's own font)"),
    ],
    '#default_value' => $brand['font'],
    '#parents' => ['brand', 'font'],
  ];
  $form['typography']['corners'] = [
    '#type' => 'radios',
    '#title' => t('Corners'),
    '#options' => [
      'sharp' => t('Sharp'),
      'soft' => t('Soft'),
      'round' => t('Round'),
    ],
    '#default_value' => $brand['corners'],
    '#parents' => ['brand', 'corners'],
  ];

  $header = (array) theme_get_setting('header');
  $form['header'] = [
    '#type' => 'details',
    '#title' => t('Header'),
    '#group' => 'seeds_site',
    '#tree' => TRUE,
  ];
  $form['header']['sticky'] = [
    '#type' => 'checkbox',
    '#title' => t('Keep the header visible when scrolling'),
    '#default_value' => $header['sticky'] ?? TRUE,
  ];
  $form['header']['topbar'] = [
    '#type' => 'checkbox',
    '#title' => t('Show a top bar with the phone number, email and social links'),
    '#description' => t('Only appears when at least one of them is filled in. Blocks in the Header region (for example a language switcher) also appear there.'),
    '#default_value' => $header['topbar'] ?? TRUE,
  ];
  $form['header']['search'] = [
    '#type' => 'checkbox',
    '#title' => t('Show a search button'),
    '#default_value' => $header['search'] ?? TRUE,
  ];
  $form['header']['search_path'] = [
    '#type' => 'textfield',
    '#title' => t('Search page'),
    '#description' => t('The page that shows search results. It receives the search words as <code>?keys=</code>.'),
    '#default_value' => $header['search_path'] ?? '/search',
    '#states' => ['visible' => [':input[name="header[search]"]' => ['checked' => TRUE]]],
  ];
  $form['header']['cta_text'] = [
    '#type' => 'textfield',
    '#title' => t('Button text'),
    '#description' => t('A highlighted button at the end of the header, for example "Donate" or "Contact us". Leave empty for no button.'),
    '#default_value' => $header['cta_text'] ?? '',
    '#maxlength' => 40,
  ];
  $form['header']['cta_url'] = [
    '#type' => 'textfield',
    '#title' => t('Button link'),
    '#description' => t('A page on this site, such as <code>/contact-us</code>, or a full URL.'),
    '#default_value' => $header['cta_url'] ?? '',
  ];

  $contact = (array) theme_get_setting('contact');
  $form['contact'] = [
    '#type' => 'details',
    '#title' => t('Contact details'),
    '#group' => 'seeds_site',
    '#tree' => TRUE,
    '#description' => t('Shown in the top bar and the footer.'),
  ];
  $form['contact']['phone'] = [
    '#type' => 'tel',
    '#title' => t('Phone'),
    '#default_value' => $contact['phone'] ?? '',
  ];
  $form['contact']['email'] = [
    '#type' => 'email',
    '#title' => t('Email'),
    '#default_value' => $contact['email'] ?? '',
  ];
  $form['contact']['address'] = [
    '#type' => 'textarea',
    '#title' => t('Address'),
    '#rows' => 3,
    '#default_value' => $contact['address'] ?? '',
  ];

  $footer = (array) theme_get_setting('footer');
  $form['footer'] = [
    '#type' => 'details',
    '#title' => t('Footer'),
    '#group' => 'seeds_site',
    '#tree' => TRUE,
  ];
  $form['footer']['about'] = [
    '#type' => 'textarea',
    '#title' => t('About text'),
    '#description' => t('One or two sentences under the logo.'),
    '#rows' => 3,
    '#default_value' => $footer['about'] ?? '',
  ];
  $form['footer']['copyright'] = [
    '#type' => 'textfield',
    '#title' => t('Copyright note'),
    '#description' => t('Shown after "© @year Site name."', ['@year' => date('Y')]),
    '#default_value' => $footer['copyright'] ?? '',
  ];
  $form['footer']['tone'] = [
    '#type' => 'radios',
    '#title' => t('Footer background'),
    '#options' => [
      'dark' => t('Dark (a deep shade of the primary colour)'),
      'primary' => t('Primary colour'),
      'light' => t('Light (the background colour)'),
    ],
    '#default_value' => $footer['tone'] ?? 'dark',
  ];
  $form['footer']['registration'] = [
    '#type' => 'textfield',
    '#title' => t('Registration note'),
    '#description' => t('For example a registered charity or company number. Shown at the bottom of the footer.'),
    '#default_value' => $footer['registration'] ?? '',
  ];
  $form['footer']['logo_white'] = [
    '#type' => 'checkbox',
    '#title' => t('Show the logo in white on the dark footer'),
    '#description' => t('Works for one-colour logos. Turn it off for multi-colour logos.'),
    '#default_value' => $footer['logo_white'] ?? TRUE,
  ];

  $form['social'] = [
    '#type' => 'details',
    '#title' => t('Social media'),
    '#group' => 'seeds_site',
    '#tree' => TRUE,
    '#description' => t('Full addresses of your pages. Empty ones are not shown.'),
  ];
  foreach (seeds_site_theme_social_networks() as $name => $label) {
    $form['social'][$name] = [
      '#type' => 'url',
      '#title' => $label,
      '#default_value' => theme_get_setting('social.' . $name) ?? '',
    ];
  }

  $form['#validate'][] = 'seeds_site_theme_settings_validate';
  $form['#submit'][] = 'seeds_site_theme_settings_submit';
}

/**
 * Validates the header button link.
 */
function seeds_site_theme_settings_validate(array &$form, FormStateInterface $form_state): void {
  $url = trim((string) $form_state->getValue(['header', 'cta_url']));
  if ($url !== '' && !preg_match('/^(\/|https?:\/\/|mailto:|tel:)/', $url)) {
    $form_state->setErrorByName('header][cta_url', t('Start the button link with a slash (/contact-us) or with https://.'));
  }
  if (trim((string) $form_state->getValue(['header', 'cta_text'])) !== '' && $url === '') {
    $form_state->setErrorByName('header][cta_url', t('Add a link for the header button, or remove its text.'));
  }
}

/**
 * Rebuilds the brand stylesheet after the settings are saved.
 */
function seeds_site_theme_settings_submit(array &$form, FormStateInterface $form_state): void {
  seeds_site_theme_brand_changed();
}
