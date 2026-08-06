## GPG Signing Key

All official ZIM releases are signed with the following key.
Any ZIM file without a valid .asc signature is not produced by Zimmaker.

- Fingerprint: 2DBE5326AA0C409B9E958C816E443E21F230376B
- Key ID:      6E443E21F230376B
- UID:         Zimmaker <kam62869536@hotmail.com>
- Created:     2026-08-05
- Algorithm:   RSA 4096
- pages1: [https://zimmaker.github.io/termux-zim-maker/](https://zimmaker.github.io/termux-zim-maker/)
- pages2:
[https://zimmaker.github.io/help/](https://zimmaker.github.io/help/)

## Public key (ASCII armored):
https://github.com/zimmaker/termux-zim-maker/releases/download/v1.0.0/zimmaker-pub.asc

Verification:
 ```
  gpg --import zimmaker-pub.asc
  gpg --verify help-zimmaker.zim.asc help-zimmaker.zim
```
Note:
  The private key is never published.
  The revocation certificate is stored offline.
  Only signatures verified against the above fingerprint should be trusted.
