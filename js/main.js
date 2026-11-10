/**
 * @file
 * Seeds site theme behaviours:
 * - Adds a shadow to the sticky header once the page scrolls.
 * - Opens gallery photos (field_media_unlimited) in a lightbox.
 *
 * Menus, the mobile drawer and header search use Alpine.js from Seeds UI.
 */
((Drupal, once) => {
  'use strict';

  Drupal.behaviors.seedsSiteHeader = {
    attach(context) {
      once('seeds-site-header', '.site-header--sticky', context).forEach((header) => {
        const sentinel = document.createElement('div');
        sentinel.setAttribute('aria-hidden', 'true');
        sentinel.style.cssText = 'position:absolute;top:0;height:1px;width:1px;';
        document.body.prepend(sentinel);
        new IntersectionObserver(([entry]) => {
          header.classList.toggle('is-scrolled', !entry.isIntersecting);
        }).observe(sentinel);
      });

      // The mobile menu drawer opens right under the header, whatever its
      // height (top bar, admin toolbar, scroll position).
      once('seeds-site-drawer', '.site-header', context).forEach((header) => {
        const update = () => {
          const bottom = Math.max(0, Math.round(header.querySelector('.site-header__main')?.getBoundingClientRect().bottom ?? 0));
          header.style.setProperty('--seeds-site-drawer-top', `${bottom}px`);
        };
        header.querySelector('.site-header__toggle')?.addEventListener('click', update);
        window.addEventListener('resize', update, { passive: true });
        window.addEventListener('scroll', update, { passive: true });
        update();
      });
    },
  };

  /**
   * Builds the lightbox <dialog> once per page.
   */
  function lightbox() {
    let dialog = document.querySelector('.seeds-site-lightbox');
    if (dialog) {
      return dialog;
    }
    const icon = (path) => `<svg class="icon" viewBox="0 0 24 24" width="24" height="24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round" aria-hidden="true">${path}</svg>`;
    dialog = document.createElement('dialog');
    dialog.className = 'seeds-site-lightbox';
    dialog.setAttribute('aria-label', Drupal.t('Photo viewer'));
    dialog.innerHTML = `
      <button type="button" class="seeds-site-lightbox__button seeds-site-lightbox__prev">${icon('<path d="M5 12h14M13 6l6 6-6 6"/>')}<span class="visually-hidden">${Drupal.t('Previous photo')}</span></button>
      <figure class="seeds-site-lightbox__figure"><img alt=""><figcaption></figcaption></figure>
      <button type="button" class="seeds-site-lightbox__button seeds-site-lightbox__next">${icon('<path d="M5 12h14M13 6l6 6-6 6"/>')}<span class="visually-hidden">${Drupal.t('Next photo')}</span></button>
      <p class="seeds-site-lightbox__count" aria-live="polite"></p>
      <button type="button" class="seeds-site-lightbox__button seeds-site-lightbox__close">${icon('<path d="M6 6l12 12M18 6 6 18"/>')}<span class="visually-hidden">${Drupal.t('Close')}</span></button>`;
    document.body.append(dialog);

    const show = (index) => {
      const photos = dialog.photos;
      dialog.index = (index + photos.length) % photos.length;
      const img = photos[dialog.index];
      const full = dialog.querySelector('img');
      full.src = img.currentSrc || img.src;
      full.alt = img.alt;
      dialog.querySelector('figcaption').textContent = img.alt;
      dialog.querySelector('.seeds-site-lightbox__count').textContent = Drupal.t('Photo @n of @total', { '@n': dialog.index + 1, '@total': photos.length });
      const single = photos.length < 2;
      dialog.querySelector('.seeds-site-lightbox__prev').hidden = single;
      dialog.querySelector('.seeds-site-lightbox__next').hidden = single;
    };
    dialog.show = show;
    dialog.querySelector('.seeds-site-lightbox__prev').addEventListener('click', () => show(dialog.index - 1));
    dialog.querySelector('.seeds-site-lightbox__next').addEventListener('click', () => show(dialog.index + 1));
    dialog.querySelector('.seeds-site-lightbox__close').addEventListener('click', () => dialog.close());
    // Click on the dark background closes the viewer.
    dialog.addEventListener('click', (event) => {
      if (event.target === dialog || event.target.classList.contains('seeds-site-lightbox__figure')) {
        dialog.close();
      }
    });
    dialog.addEventListener('keydown', (event) => {
      const rtl = document.documentElement.dir === 'rtl';
      if (event.key === 'ArrowRight') {
        show(dialog.index + (rtl ? -1 : 1));
      }
      if (event.key === 'ArrowLeft') {
        show(dialog.index + (rtl ? 1 : -1));
      }
    });
    dialog.addEventListener('close', () => dialog.opener?.focus());
    return dialog;
  }

  Drupal.behaviors.seedsSiteLightbox = {
    attach(context) {
      once('seeds-site-lightbox', '.field--name-field-media-unlimited', context).forEach((gallery) => {
        const photos = [...gallery.querySelectorAll('.field--name-field-image img, .field--type-image img')];
        photos.forEach((img, index) => {
          const button = document.createElement('button');
          button.type = 'button';
          button.className = 'seeds-site-lightbox-trigger';
          button.setAttribute('aria-label', Drupal.t('Open photo @n of @total', { '@n': index + 1, '@total': photos.length }));
          img.replaceWith(button);
          button.append(img);
          button.addEventListener('click', () => {
            const dialog = lightbox();
            dialog.photos = photos;
            dialog.opener = button;
            dialog.show(index);
            dialog.showModal();
          });
        });
      });
    },
  };
})(Drupal, once);
