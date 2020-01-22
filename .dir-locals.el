((prog-mode . ((lsp-file-watch-threshold . nil)
	       (eval . (progn (lsp)
			      (yas-minor-mode-on)
			      (electric-pair-mode t))))))
