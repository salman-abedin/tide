# Maintainer: Salman Abedin <salmanabedin@disroot.org>
pkgname=tide-git
_author=salman-abedin
_source=github
pkgver=10.0
pkgrel=1
pkgdesc="Minimal TUI for the BitTorrent client Transmission"
arch=(x86_64 i686)
license=('GPL')
depends=(ncurses transmission-cli)
makedepends=(git)
source=("git+https://$_source.com/$_author/$pkgname")
md5sums=('SKIP')

build() {
   cd "${pkgname%-*}"
   make
}

package() {
   cd "${pkgname%-*}"
   sudo make install
}
