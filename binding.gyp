{
    "targets": [{
        "target_name": "binding",
        "sources": ["unistd.cc"],
        "include_dirs": ["<!(node -e \"require('nan')\")"]
    }]
}
