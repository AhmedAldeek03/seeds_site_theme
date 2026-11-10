<?php

/**
 * @file
 * Brand colours, font and corner style from the theme settings.
 *
 * Four colours set the whole palette (Appearance > Settings > Seeds site
 * theme > Colours):
 * - Primary: buttons, links, section titles, the Primary background.
 * - Secondary: button hover, tags, the Secondary and Light secondary
 *   backgrounds.
 * - Accent: the short line under titles, icons and other small touches.
 * - Background: light sections and cards (a warm off-white by default).
 *
 * Light tints, a dark shade and text colours are derived from them. Text
 * colours are darkened until they reach 4.5:1 (WCAG AA) on every light
 * background they are used on, and text on coloured backgrounds is white or
 * dark, whichever reads better.
 *
 * The compiled CSS (dist/css) holds the default palette. When the settings
 * differ, the theme writes public://seeds_site_theme/brand-HASH.css with the
 * matching custom properties and adds it to the global library, so the front
 * end and the Canvas editor preview use the same colours.
 *
 * scss/_variables.scss derives the default palette with the same formulas.
 */

declare(strict_types=1);

/**
 * Default brand settings. Keep in sync with config/install and _variables.scss.
 */
const SEEDS_SITE_THEME_BRAND_DEFAULTS = [
  'primary' => '#14406b',
  'secondary' => '#1f6f78',
  'accent' => '#c29a4a',
  'surface' => '#f8f5ef',
  'headings' => 'primary',
  'font' => 'plex',
  'corners' => 'soft',
];

/**
 * Text colour for headings and body copy on light backgrounds.
 */
const SEEDS_SITE_THEME_INK = '#18222f';

/**
 * Near-black the dark shade is mixed from.
 */
const SEEDS_SITE_THEME_NIGHT = '#070c14';

/**
 * Corner radii (small, default, large) for each corner style.
 */
const SEEDS_SITE_THEME_CORNERS = [
  'sharp' => ['2px', '4px', '6px'],
  'soft' => ['8px', '12px', '16px'],
  'round' => ['12px', '20px', '28px'],
];

/**
 * Font stacks offered in the theme settings.
 */
function seeds_site_theme_font_stacks(): array {
  return [
    'plex' => '"IBM Plex Sans", "IBM Plex Sans Arabic", system-ui, -apple-system, "Segoe UI", Roboto, sans-serif',
    'cairo' => '"Cairo", system-ui, -apple-system, "Segoe UI", Roboto, sans-serif',
    'system' => 'system-ui, -apple-system, "Segoe UI", Roboto, "Noto Sans", "Noto Sans Arabic", Tahoma, sans-serif',
  ];
}

/**
 * Returns a valid #rrggbb colour, or the fallback.
 */
function seeds_site_theme_normalize_hex(?string $hex, string $fallback): string {
  $hex = strtolower(trim((string) $hex));
  if (preg_match('/^#?([0-9a-f]{3})$/', $hex, $m)) {
    $hex = '#' . $m[1][0] . $m[1][0] . $m[1][1] . $m[1][1] . $m[1][2] . $m[1][2];
  }
  if (preg_match('/^#?([0-9a-f]{6})$/', $hex, $m)) {
    return '#' . $m[1];
  }
  return $fallback;
}

/**
 * Converts #rrggbb to [r, g, b].
 */
function seeds_site_theme_hex_to_rgb(string $hex): array {
  $hex = ltrim($hex, '#');
  return [hexdec(substr($hex, 0, 2)), hexdec(substr($hex, 2, 2)), hexdec(substr($hex, 4, 2))];
}

/**
 * Converts [r, g, b] to #rrggbb.
 */
function seeds_site_theme_rgb_to_hex(array $rgb): string {
  return '#' . implode('', array_map(static fn ($c) => str_pad(dechex((int) round(max(0, min(255, $c)))), 2, '0', STR_PAD_LEFT), $rgb));
}

/**
 * Mixes two colours like Sass color.mix(): $weight is the share of $a (0-1).
 */
function seeds_site_theme_mix(string $a, string $b, float $weight): string {
  $ra = seeds_site_theme_hex_to_rgb($a);
  $rb = seeds_site_theme_hex_to_rgb($b);
  $out = [];
  foreach ([0, 1, 2] as $i) {
    $out[] = $ra[$i] * $weight + $rb[$i] * (1 - $weight);
  }
  return seeds_site_theme_rgb_to_hex($out);
}

/**
 * Relative luminance (WCAG 2).
 */
function seeds_site_theme_luminance(string $hex): float {
  $channels = array_map(static function ($c) {
    $c /= 255;
    return $c <= 0.03928 ? $c / 12.92 : (($c + 0.055) / 1.055) ** 2.4;
  }, seeds_site_theme_hex_to_rgb($hex));
  return 0.2126 * $channels[0] + 0.7152 * $channels[1] + 0.0722 * $channels[2];
}

/**
 * Contrast ratio between two colours (1 to 21).
 */
function seeds_site_theme_contrast(string $a, string $b): float {
  $la = seeds_site_theme_luminance($a);
  $lb = seeds_site_theme_luminance($b);
  return (max($la, $lb) + 0.05) / (min($la, $lb) + 0.05);
}

/**
 * Darkens a colour in 5% steps until it reaches $ratio on every background.
 */
function seeds_site_theme_readable_on(string $color, array $backgrounds, float $ratio = 4.5): string {
  $readable = static function (string $candidate) use ($backgrounds, $ratio): bool {
    foreach ($backgrounds as $background) {
      if (seeds_site_theme_contrast($candidate, $background) < $ratio) {
        return FALSE;
      }
    }
    return TRUE;
  };
  $candidate = $color;
  for ($step = 1; $step <= 20 && !$readable($candidate); $step++) {
    $candidate = seeds_site_theme_mix('#000000', $color, $step * 0.05);
  }
  return $candidate;
}

/**
 * White or ink, whichever reads better on a colour (white wins at 4.5:1).
 */
function seeds_site_theme_text_on(string $background): string {
  return seeds_site_theme_contrast('#ffffff', $background) >= 4.5 ? '#ffffff' : SEEDS_SITE_THEME_INK;
}

/**
 * Brand settings of the theme, with defaults.
 */
function seeds_site_theme_brand_settings(string $theme = 'seeds_site_theme'): array {
  $brand = (array) theme_get_setting('brand', $theme) + SEEDS_SITE_THEME_BRAND_DEFAULTS;
  $defaults = SEEDS_SITE_THEME_BRAND_DEFAULTS;
  return [
    'primary' => seeds_site_theme_normalize_hex($brand['primary'] ?? NULL, $defaults['primary']),
    'secondary' => seeds_site_theme_normalize_hex($brand['secondary'] ?? NULL, $defaults['secondary']),
    'accent' => seeds_site_theme_normalize_hex($brand['accent'] ?? NULL, $defaults['accent']),
    'surface' => seeds_site_theme_normalize_hex($brand['surface'] ?? NULL, $defaults['surface']),
    'headings' => in_array($brand['headings'] ?? '', ['primary', 'dark'], TRUE) ? $brand['headings'] : $defaults['headings'],
    'font' => isset(seeds_site_theme_font_stacks()[$brand['font'] ?? '']) ? $brand['font'] : $defaults['font'],
    'corners' => isset(SEEDS_SITE_THEME_CORNERS[$brand['corners'] ?? '']) ? $brand['corners'] : $defaults['corners'],
  ];
}

/**
 * The derived palette of a brand: every colour token, without the prefix.
 *
 * Formulas match scss/_variables.scss.
 */
function seeds_site_theme_palette(array $brand): array {
  $white = '#ffffff';
  $ink = SEEDS_SITE_THEME_INK;
  $primary = $brand['primary'];
  $secondary = $brand['secondary'];
  $accent = $brand['accent'];
  $surface = $brand['surface'];

  $palette = [
    'primary' => $primary,
    'primary-light' => seeds_site_theme_mix($primary, $white, 0.07),
    'primary-line' => seeds_site_theme_mix($primary, $white, 0.2),
    'secondary' => $secondary,
    'secondary-light' => seeds_site_theme_mix($secondary, $white, 0.09),
    'secondary-line' => seeds_site_theme_mix($secondary, $white, 0.2),
    'accent' => $accent,
    'accent-hover' => seeds_site_theme_mix('#000000', $accent, 0.12),
    'accent-light' => seeds_site_theme_mix($accent, $white, 0.14),
    'accent-line' => seeds_site_theme_mix($accent, $white, 0.32),
    'surface' => $surface,
    'surface-2' => seeds_site_theme_mix($accent, $surface, 0.08),
    'dark' => seeds_site_theme_mix($primary, SEEDS_SITE_THEME_NIGHT, 0.42),
    'dark-2' => seeds_site_theme_mix($primary, SEEDS_SITE_THEME_NIGHT, 0.55),
  ];

  // Text in a brand colour must stay readable on all the light backgrounds.
  $light_backgrounds = [$white, $surface, $palette['surface-2'], $palette['primary-light'], $palette['secondary-light'], $palette['accent-light']];
  $palette['primary-text'] = seeds_site_theme_readable_on($primary, $light_backgrounds);
  $palette['secondary-text'] = seeds_site_theme_readable_on($secondary, $light_backgrounds);
  $palette['heading'] = $brand['headings'] === 'dark' ? $ink : $palette['primary-text'];

  // Text on brand colours: white or dark.
  $palette['on-primary'] = seeds_site_theme_text_on($primary);
  $palette['on-secondary'] = seeds_site_theme_text_on($secondary);
  $palette['on-accent'] = seeds_site_theme_text_on($accent);
  return $palette;
}

/**
 * CSS custom properties for a brand.
 */
function seeds_site_theme_brand_tokens(array $brand): array {
  $tokens = [];
  foreach (seeds_site_theme_palette($brand) as $name => $value) {
    $tokens['--seeds-site-' . $name] = $value;
  }
  $tokens['--seeds-site-font'] = seeds_site_theme_font_stacks()[$brand['font']];
  [$small, $default, $large] = SEEDS_SITE_THEME_CORNERS[$brand['corners']];
  $tokens['--seeds-site-radius-sm'] = $small;
  $tokens['--seeds-site-radius'] = $default;
  $tokens['--seeds-site-radius-lg'] = $large;
  return $tokens;
}

/**
 * The brand stylesheet for the current settings, or '' for the default brand.
 */
function seeds_site_theme_brand_css(string $theme = 'seeds_site_theme'): string {
  $brand = seeds_site_theme_brand_settings($theme);
  if ($brand == SEEDS_SITE_THEME_BRAND_DEFAULTS) {
    return '';
  }
  $lines = [];
  foreach (seeds_site_theme_brand_tokens($brand) as $property => $value) {
    $lines[] = "  $property: $value;";
  }
  return "/* Generated by the Seeds site theme from its colour settings. Do not edit. */\n:root {\n" . implode("\n", $lines) . "\n}\n";
}

/**
 * Writes the brand stylesheet if needed and returns its URI, or NULL.
 *
 * The file name holds a hash of its content, so browsers never keep an old
 * brand and the file is only written once per brand.
 */
function seeds_site_theme_brand_css_uri(string $theme = 'seeds_site_theme'): ?string {
  $css = seeds_site_theme_brand_css($theme);
  if ($css === '') {
    return NULL;
  }
  $directory = 'public://seeds_site_theme';
  $uri = $directory . '/brand-' . substr(hash('sha256', $css), 0, 12) . '.css';
  if (!file_exists($uri)) {
    /** @var \Drupal\Core\File\FileSystemInterface $file_system */
    $file_system = \Drupal::service('file_system');
    if (!$file_system->prepareDirectory($directory, $file_system::CREATE_DIRECTORY | $file_system::MODIFY_PERMISSIONS)) {
      return NULL;
    }
    try {
      $file_system->saveData($css, $uri, \Drupal\Core\File\FileExists::Replace);
    }
    catch (\Throwable $e) {
      \Drupal::logger('seeds_site_theme')->error('Could not write the brand stylesheet: @message', ['@message' => $e->getMessage()]);
      return NULL;
    }
  }
  return $uri;
}

/**
 * Clears the cached libraries so a new brand stylesheet is picked up.
 *
 * Called when the theme settings form is saved. Here, not in the .theme file:
 * the settings form runs in the admin theme, where the .theme file of this
 * theme is not loaded. After changing the settings through config import,
 * clear the caches instead (drush deploy does).
 */
function seeds_site_theme_brand_changed(): void {
  \Drupal\Core\Cache\Cache::invalidateTags(['library_info', 'config:seeds_site_theme.settings']);
  \Drupal::service('asset.css.collection_optimizer')->deleteAll();
}
