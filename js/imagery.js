$(document).ready( function() {
        $('.imagery a[data-toggle="tab"]').on("hide.bs.tab", function(event) {
            if(history.pushState)
            {
                history.pushState(null, null, '#/imagery/' + event.relatedTarget.dataset.target.substring(1));
                console.log('#/imagery/' + event.relatedTarget.dataset.target.substring(1));
            }
            else
            {
                location.hash = '#/imagery/' + event.target;
                console.log('#/imagery/' + event.target);
            }
        });
    });
