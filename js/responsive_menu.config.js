/**
 * Responsive Menu 7.x-3.7, modified October 2026.
 * Adds header-relative bottom-dropdown navigation; see NOTICE.md.
 */
(function ($) {
  'use strict';

  Drupal.behaviors.responsive_menu = {
    attach: function (context) {
      $('body', context).once('responsive-menu', function () {
        var settings = Drupal.settings.responsive_menu || {};
        if (settings.position === 'bottom') {
          initialiseBottomMenu(settings);
        }
        else if (typeof Mmenu !== 'undefined') {
          initialiseMmenu(settings);
        }

        if ($.fn.superfish) {
          var sf = settings.superfish || {};
          $('#horizontal-menu').superfish({
            delay: parseInt(sf.delay, 10) || 300,
            speed: parseInt(sf.speed, 10) || 100,
            speedOut: parseInt(sf.speedOut, 10) || 100
          }).addClass('sf-menu');
        }
      });
    }
  };

  function initialiseMmenu(settings) {
    var position = settings.position || 'left';
    var options = {
      extensions: [
        settings.theme || 'theme-dark',
        'pagedim-black',
        position === 'left' ? 'position-left' : 'position-right'
      ],
      drag: !!settings.drag,
      keyboardNavigation: { enable: true, enhance: true }
    };
    var config = { clone: false, classNames: { selected: 'active' } };
    if (settings.custom) {
      if (settings.custom.options) {
        $.extend(true, options, settings.custom.options);
      }
      if (settings.custom.config) {
        $.extend(true, config, settings.custom.config);
      }
    }
    var mmenu = new Mmenu('#off-canvas', options, config);
    $('#toggle-icon').on('click.responsiveMmenu', function (event) {
      event.preventDefault();
      if (mmenu.API && typeof mmenu.API.open === 'function') {
        mmenu.API.open();
      }
    });
  }

  function initialiseBottomMenu(settings) {
    var $wrapper = $('.off-canvas-wrapper');
    var $menu = $('#off-canvas');
    var $toggle = $('#toggle-icon');
    var $root = $('#off-canvas-menu');
    var $anchor = $(settings.bottomAnchor || '#tools, #inner-tools').filter(':visible').first();
    var $containingHeader = $toggle.closest('#tools, #inner-tools, header, .header, [role="banner"]');
    var stack = [];

    function focusWithoutScrolling($element) {
      var element = $element && $element.get(0);
      if (!element) {
        return;
      }
      try {
        element.focus({ preventScroll: true });
      }
      catch (error) {
        // Older browsers do not accept focus options. Keeping focus where it
        // is is preferable to moving the entire document.
      }
    }

    // Themes may use different header containers on the homepage and inner
    // pages. Prefer the header that actually contains the toggle, especially
    // when a configured selector resolves to an unrelated utility element.
    if ($containingHeader.length && (!$anchor.length || $anchor.outerWidth() < $(window).width() * 0.5)) {
      $anchor = $containingHeader.first();
    }
    if (!$anchor.length) {
      $anchor = $toggle.parent();
    }

    if ((window.location.hash === '#off-canvas' || window.location.hash === '#mm-0') && window.history && window.history.replaceState) {
      window.history.replaceState(null, document.title, window.location.pathname + window.location.search);
    }

    $wrapper.addClass('rm-bottom-menu').css('--rm-columns', parseInt(settings.bottomColumns, 10) || 4);
    $menu.attr({ role: 'navigation', 'aria-label': Drupal.t('Main menu') });
    $root.addClass('rm-panel rm-panel-active').attr('aria-hidden', 'false');

    var $subPanels = $root.find('ul');
    $subPanels.each(function () {
      var $panel = $(this);
      var $parentItem = $panel.parent('li');
      var $parentLink = $parentItem.children('a').first();
      var $parentPanel = $parentItem.closest('ul');
      var $plainLabel = $parentItem.children('span').first();
      var $submenuButton;
      var title;

      // A real menu link remains navigable. Its separate arrow opens the
      // submenu. Drupal <nolink>/separator items keep a plain label, so only
      // their arrow is interactive, matching the original module behaviour.
      if ($parentLink.length) {
        title = $.trim($parentLink.text());
      }
      else if ($plainLabel.length) {
        title = $.trim($plainLabel.text());
        var $separatorButton = $('<button type="button" class="rm-submenu-label"></button>').text(title);
        $plainLabel.replaceWith($separatorButton);
        $plainLabel = $separatorButton;
      }
      else {
        title = $.trim($parentItem.clone().children().remove().end().text());
        $parentItem.contents().filter(function () {
          return this.nodeType === 3;
        }).remove();
        $plainLabel = $('<button type="button" class="rm-submenu-label"></button>').text(title).prependTo($parentItem);
      }

      $submenuButton = $('<button type="button" class="rm-submenu-arrow"></button>')
        .attr({
          'aria-label': Drupal.t('Open @title submenu', { '@title': title }),
          'aria-haspopup': 'true',
          'aria-expanded': 'false'
        })
        .insertBefore($panel);

      var $back = $('<li class="rm-menu-back"><button type="button"><span aria-hidden="true">&#8592;</span> ' + Drupal.checkPlain(title) + '</button></li>');
      $panel.addClass('rm-panel').attr('aria-hidden', 'true').prepend($back);
      $parentItem.addClass('rm-has-submenu');
      $submenuButton.data('rm-submenu', $panel);
      if (!$parentLink.length) {
        $plainLabel
          .attr({ 'aria-haspopup': 'true', 'aria-expanded': 'false' })
          .data('rm-submenu', $panel);
      }
      $panel.data('rm-parent-panel', $parentPanel);
      $panel.data('rm-parent-trigger', $submenuButton);
      $panel.data('rm-parent-item', $parentItem);
    });

    // Panels must be siblings. A transformed/transparent parent panel would
    // otherwise also transform and hide every submenu nested inside it.
    $subPanels.detach().appendTo($menu);

    // Open at the deepest active-trail panel so an inner page reflects its
    // position in the menu instead of always starting at the homepage level.
    var $activeTrailPanels = $subPanels.filter(function () {
      return $(this).data('rm-parent-item').hasClass('active-trail');
    });
    if ($activeTrailPanels.length) {
      $root.removeClass('rm-panel-active').addClass('rm-panel-parent').attr('aria-hidden', 'true');
      $activeTrailPanels.each(function (index) {
        var $panel = $(this);
        var isCurrent = index === $activeTrailPanels.length - 1;
        $panel
          .toggleClass('rm-panel-parent', !isCurrent)
          .toggleClass('rm-panel-active', isCurrent)
          .attr('aria-hidden', isCurrent ? 'false' : 'true');
        $panel.data('rm-parent-trigger')
          .attr('aria-expanded', 'true')
          .siblings('.rm-submenu-label').attr('aria-expanded', 'true');
        stack.push($panel);
      });
    }

    function layoutPanels() {
      var width = $anchor.outerWidth() || $(window).width();
      var columns = parseInt(settings.bottomColumns, 10) || 4;
      if (width <= 480) {
        columns = 1;
      }
      else if (width <= 760) {
        columns = Math.min(columns, 2);
      }
      else if (width <= 1000) {
        columns = Math.min(columns, 3);
      }

      $menu.find('ul.rm-panel').each(function () {
        var $panel = $(this);
        var $items = $panel.children('li').not('.rm-menu-back');
        var rows = Math.ceil($items.length / columns) || 1;
        var rowOffset = $panel.children('.rm-menu-back').length ? 1 : 0;
        $panel.css('--rm-active-columns', columns);
        $items.each(function (index) {
          // Force column-major placement: fill each column from top to bottom
          // before moving to the next column. Theme nth-child rules otherwise
          // make CSS Grid fall back to its default left-to-right order.
          this.style.setProperty('grid-column', Math.floor(index / rows) + 1, 'important');
          this.style.setProperty('grid-row', (index % rows) + 1 + rowOffset, 'important');
        });
      });
    }

    function placeMenu() {
      var offset = $anchor.offset();
      if (offset) {
        $wrapper.css({
          left: offset.left,
          top: offset.top + $anchor.outerHeight(),
          width: $anchor.outerWidth()
        });
      }
    }

    function setMenuHeight() {
      window.requestAnimationFrame(function () {
        var height = 0;
        $menu.find('ul.rm-panel').each(function () {
          var $panel = $(this).addClass('rm-panel-measuring');
          height = Math.max(height, this.scrollHeight);
          $panel.removeClass('rm-panel-measuring');
        });
        $menu.css('height', height);
      });
    }

    function openMenu() {
      placeMenu();
      $wrapper.addClass('rm-menu-open').attr('aria-hidden', 'false');
      $('body').addClass('rm-bottom-menu-open');
      $toggle.attr('aria-expanded', 'true');
      setMenuHeight();
    }

    function closeMenu(restoreFocus) {
      $wrapper.removeClass('rm-menu-open').attr('aria-hidden', 'true');
      $('body').removeClass('rm-bottom-menu-open');
      $toggle.attr('aria-expanded', 'false');
      if (restoreFocus !== false) {
        focusWithoutScrolling($toggle);
      }
    }

    function showPanel($panel) {
      var $current = stack.length ? stack[stack.length - 1] : $root;
      layoutPanels();
      $current.removeClass('rm-panel-active rm-panel-returning').addClass('rm-panel-parent').attr('aria-hidden', 'true');
      $panel.css('display', '').removeClass('rm-panel-parent rm-panel-returning').addClass('rm-panel-active').attr('aria-hidden', 'false');
      stack.push($panel);
      focusWithoutScrolling($panel.find('a, button').filter(':visible').first());
    }

    function goBack() {
      if (!stack.length) {
        closeMenu();
        return;
      }
      var $current = stack.pop();
      var $parent = $current.data('rm-parent-panel') || $root;
      var $parentTrigger = $current.data('rm-parent-trigger');

      // Safari can retain the outgoing panel's composited animation layer for
      // one paint, briefly flashing only the Back header. An explicit inline
      // display reset removes it from layout before the parent is activated.
      $current.removeClass('rm-panel-active').css('display', 'none').attr('aria-hidden', 'true');
      $parent.css('display', '').removeClass('rm-panel-parent').addClass('rm-panel-active rm-panel-returning').attr('aria-hidden', 'false');
      $parentTrigger.attr('aria-expanded', 'false');
      $parentTrigger.siblings('.rm-submenu-label').attr('aria-expanded', 'false');
      focusWithoutScrolling($parentTrigger);
    }

    $wrapper.attr('aria-hidden', 'true');
    layoutPanels();
    placeMenu();
    $(window).on('resize.rmBottomMenu', function () {
      layoutPanels();
      placeMenu();
      if ($wrapper.hasClass('rm-menu-open')) {
        setMenuHeight();
      }
    });
    $(window).on('scroll.rmBottomMenu', placeMenu);

    $toggle.on('click.rmBottomMenu', function (event) {
      event.preventDefault();
      $wrapper.hasClass('rm-menu-open') ? closeMenu() : openMenu();
    });

    $menu.on('click.rmBottomMenu', '.rm-submenu-arrow, .rm-submenu-label', function (event) {
      var $button = $(this);
      var $submenu = $button.data('rm-submenu');
      event.preventDefault();
      event.stopPropagation();
      $button.closest('li').children('.rm-submenu-arrow, .rm-submenu-label').attr('aria-expanded', 'true');
      showPanel($submenu);
    });

    $menu.on('click.rmBottomMenu', 'a', function (event) {
      var href = $(this).attr('href') || '';
      if (href === '#' || href === '#off-canvas' || href === '#mm-0') {
        event.preventDefault();
      }
      else {
        // Do not return focus to the toggle before navigation: focusing an
        // off-screen header control causes browsers to smooth-scroll upward.
        closeMenu(false);
      }
    });

    $menu.on('click.rmBottomMenu', '.rm-menu-back > button', goBack);

    $(document).on('keydown.rmBottomMenu', function (event) {
      if ($wrapper.hasClass('rm-menu-open') && event.keyCode === 27) {
        event.preventDefault();
        stack.length ? goBack() : closeMenu();
      }
    });

    $(document).on('click.rmBottomMenu', function (event) {
      if ($wrapper.hasClass('rm-menu-open') && !$(event.target).closest('.rm-bottom-menu, #toggle-icon').length) {
        closeMenu();
      }
    });
  }
})(jQuery);
