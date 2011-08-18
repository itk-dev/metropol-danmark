(function ($) {
  var fpa = {
    table_selector: '#permissions',
    row_selector: 'tbody tr',
    filter_selector: 'td.permission',
    grouping_selector: 'td.module',
    groups: {}
  };
  var indexed_rows = {};
  
  fpa.prepare = function (context) {
    fpa.table = $(fpa.table_selector, context);
    fpa.rows = fpa.table.find(fpa.row_selector);
    
    var module_id = '';
    fpa.rows.each(function () {
      $this = $(this);
      var new_module_id = $this.find(fpa.grouping_selector).text().toLowerCase();
      if (new_module_id) {
        fpa.groups[new_module_id] = this;
        module_id = new_module_id;
      }
      $this.attr('fpa-module', module_id).attr('fpa-permission', $this.find(fpa.filter_selector).text().toLowerCase());
    });
    $('<input id="fpa_search" type="text" class="form-text" />')
      .insertBefore(fpa.table.parent())
      .keypress(function (e) {
        //prevent enter key from submitting form
        if (e.which == 13) {
          return false;
        }
      })
      .keyup(function (e) {
        var $val = $(this).val();
        if ($val != '') {
          Drupal.settings.fpa.perm = $val;
          fpa.filter(window.document);
        }
        else {
          fpa.rows
            .css('display', '')
            .removeClass('odd even')
            .filter(":even").addClass('odd').end()
            .filter(":odd").addClass('even').end();
        }
      })
      .wrap('<div class="form-item" />')
      .before('<label for="fpa_search">Search:</label>')
      .after('<div class="description">Start typing and only permissions that contain the entered text will be displayed.</div>')
      .val(Drupal.settings.fpa.perm);
  };
  
  fpa.filter = function () {
    if (typeof Drupal.settings.fpa.perm != 'undefined' && Drupal.settings.fpa.perm.length > 0) {
      var perm_copy = Drupal.settings.fpa.perm.toLowerCase().split('@');
      var perm_labels = [];
      fpa.rows.css('display', 'none');
      var selector = 'tr';
      selector += perm_copy[0] ? '[fpa-permission*="' + perm_copy[0] + '"]' : '';
      selector += perm_copy[1] ? '[fpa-module*="' + perm_copy[1] + '"]' : '';
      var items = fpa.rows.filter(selector).each(function (index, Element) {
        perm_labels.push(fpa.groups[$(this).attr('fpa-module')]);
      });
      perm_labels = $.unique(perm_labels);
      items.add(perm_labels).css('display', '')
        .removeClass('odd even')
        .filter(":even").addClass('odd').end()
        .filter(":odd").addClass('even').end();
    }
  };
  
  fpa.modalframe = function (context) {
    $('a.fpa_modalframe', context).click(function (e) {
      e.preventDefault();
      e.stopPropagation();
      Drupal.modalFrame.open({
        url: $(this).attr('href'),
        draggable: false
      });
    });
  };
  
  Drupal.behaviors.fpa = function (context) {
    fpa.prepare(context);
    fpa.filter();
    fpa.modalframe(context);
  };
})(jQuery);
