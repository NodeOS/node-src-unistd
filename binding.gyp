{
  "targets": [
    {
      "target_name": "binding",
      "sources": [ "src/unistd.cc" ],
      "include_dirs": [ "<!(node -e \"require('nan')\")" ]
    }
  ]
}
