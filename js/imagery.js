$(document).ready( function() {
        $('.imagery a[data-toggle="tab"]').on("hide.bs.tab", function(event) {
            history.replaceState(null, null, '#/imagery/' + event.relatedTarget.dataset.target.substring(1));
        });
    });
