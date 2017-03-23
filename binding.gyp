{
  "targets": [
    {
      'target_name': 'nuclearnetjs',
      'include_dirs': [
        "<!(node -e \"require('nan')\")",
        'src'
      ],
      'sources': [
        'src/nuclearnetjs.cpp'
      ]
    }
  ]
}