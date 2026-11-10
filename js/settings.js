/**
 * @file
 * Updates the colour preview on the theme settings page as colours change.
 *
 * Same formulas as seeds_site_theme_palette() in includes/brand.inc.
 */
((Drupal, once) => {
  'use strict';

  const INK = '#18222f';
  const NIGHT = '#070c14';

  const rgb = (hex) => [1, 3, 5].map((i) => parseInt(hex.slice(i, i + 2), 16));
  const hex = (c) => `#${c.map((v) => Math.round(Math.max(0, Math.min(255, v))).toString(16).padStart(2, '0')).join('')}`;
  const mix = (a, b, w) => {
    const ra = rgb(a);
    const rb = rgb(b);
    return hex(ra.map((v, i) => v * w + rb[i] * (1 - w)));
  };
  const luminance = (color) => {
    const [r, g, b] = rgb(color).map((v) => {
      const c = v / 255;
      return c <= 0.03928 ? c / 12.92 : ((c + 0.055) / 1.055) ** 2.4;
    });
    return 0.2126 * r + 0.7152 * g + 0.0722 * b;
  };
  const contrast = (a, b) => {
    const [x, y] = [luminance(a), luminance(b)].sort((m, n) => n - m);
    return (x + 0.05) / (y + 0.05);
  };
  const readable = (color, backgrounds) => {
    let candidate = color;
    for (let step = 1; step <= 20 && backgrounds.some((bg) => contrast(candidate, bg) < 4.5); step++) {
      candidate = mix('#000000', color, step * 0.05);
    }
    return candidate;
  };
  const textOn = (bg) => (contrast('#ffffff', bg) >= 4.5 ? '#ffffff' : INK);

  const palette = ({ primary, secondary, accent, surface, headings }) => {
    const p = {
      primary,
      'primary-light': mix(primary, '#ffffff', 0.07),
      secondary,
      'secondary-light': mix(secondary, '#ffffff', 0.09),
      accent,
      'accent-light': mix(accent, '#ffffff', 0.14),
      surface,
      'surface-2': mix(accent, surface, 0.08),
      dark: mix(primary, NIGHT, 0.42),
    };
    const light = ['#ffffff', surface, p['surface-2'], p['primary-light'], p['secondary-light'], p['accent-light']];
    p['primary-text'] = readable(primary, light);
    p['secondary-text'] = readable(secondary, light);
    p.heading = headings === 'dark' ? INK : p['primary-text'];
    p['on-primary'] = textOn(primary);
    p['on-secondary'] = textOn(secondary);
    return p;
  };

  Drupal.behaviors.seedsSiteSettingsPreview = {
    attach(context) {
      once('seeds-site-preview', '[data-seeds-site-preview]', context).forEach((preview) => {
        const form = preview.closest('form');
        const value = (name) => form.querySelector(`[name="brand[${name}]"]${name === 'headings' ? ':checked' : ''}`)?.value;
        const update = () => {
          const colors = Object.fromEntries(['primary', 'secondary', 'accent', 'surface', 'headings'].map((name) => [name, value(name)]));
          if (!/^#[0-9a-f]{6}$/i.test(colors.primary || '')) {
            return;
          }
          Object.entries(palette(colors)).forEach(([name, color]) => preview.style.setProperty(`--p-${name}`, color));
        };
        form.addEventListener('input', (event) => {
          if (event.target.name?.startsWith('brand[')) {
            update();
          }
        });
        form.addEventListener('change', update);
      });
    },
  };
})(Drupal, once);
