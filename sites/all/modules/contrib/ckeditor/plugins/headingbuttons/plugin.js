(function() {
  var pCommand = {
    exec:function(editor) {
      var format = {
        element : 'p'
      };
      var style = new CKEDITOR.style(format);
      style.apply(editor.document);
    }
  };
  var h1Command = {
    exec:function(editor) {
      var format = {
        element : 'h1'
      };
      var style = new CKEDITOR.style(format);
      style.apply(editor.document);
    }
  };
  var h2Command = {
    exec:function(editor) {
      var format = {
        element : 'h2'
      };
      var style = new CKEDITOR.style(format);
      style.apply(editor.document);
    }
  };
  var h3Command = {
    exec:function(editor) {
      var format = {
        element : 'h3'
      };
      var style = new CKEDITOR.style(format);
      style.apply(editor.document);
    }
  };
  var h4Command = {
    exec:function(editor) {
      var format = {
        element : 'h4'
      };
      var style = new CKEDITOR.style(format);
      style.apply(editor.document);
    }
  };
  var h5Command = {
    exec:function(editor) {
      var format = {
        element : 'h5'
      };
      var style = new CKEDITOR.style(format);
      style.apply(editor.document);
    }
  };
  var h6Command = {
    exec:function(editor) {
      var format = {
        element : 'h6'
      };
      var style = new CKEDITOR.style(format);
      style.apply(editor.document);
    }
  };

  CKEDITOR.plugins.add('headingbuttons',
  {
    init:function(editor) {
      editor.addCommand('pCommand', pCommand);
      editor.addCommand('h1Command', h1Command);
      editor.addCommand('h2Command', h2Command);
      editor.addCommand('h3Command', h3Command);
      editor.addCommand('h4Command', h4Command);
      editor.addCommand('h5Command', h5Command);
      editor.addCommand('h6Command', h6Command);
      
      editor.ui.addButton('Paragraph', {
        label:'Paragraph',
        icon: this.path + 'images/p.png',
        command:'pCommand'
      });
      editor.ui.addButton('H1', {
        label:'Heading (H1)',
        icon: this.path + 'images/h1.png',
        command:'h1Command'
      });
      editor.ui.addButton('H2', {
        label:'Heading (H2)',
        icon: this.path + 'images/h2.png',
        command:'h2Command'
      });
      editor.ui.addButton('H3', {
        label:'Heading (H3)',
        icon: this.path + 'images/h3.png',
        command:'h3Command'
      });
      editor.ui.addButton('H4', {
        label:'Heading (H4)',
        icon: this.path + 'images/h4.png',
        command:'h4Command'
      });
      editor.ui.addButton('H5', {
        label:'Heading (H5)',
        icon: this.path + 'images/h5.png',
        command:'h5Command'
      });
      editor.ui.addButton('H6', {
        label:'Heading (H6)',
        icon: this.path + 'images/h6.png',
        command:'h6Command'
      });
    }
  });
})();