$(document).ready( function() {
        $('.imagery a[data-toggle="tab"]').on("hidden.bs.tab", function(event) {
            console.log('#' + event.relatedTarget);
            if(history.pushState)
            {
                history.pushState(null, null, '#imagery/' + event.relatedTarget.dataset.target.substring(1));
            }
            else
            {
                location.hash = '#imagery/' + event.target;
            }
        });
    });
